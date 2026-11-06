# gonmakev2

[gonmake](https://github.com/gonterao-crypto/gonmake)の後継版、

日本製のC++makeツール

## 必要なもの
* Clang++（任意）

* ccache（任意）

* mingw32（任意）

* windows環境（任意、他の環境で使う場合はその環境に合わせてコンパイルすること。windowsベースに開発は進めていくので注意。）

* ~~パソコン~~

* ~~実行しても落ちないほどのメモリや電源、ストレージ、CPU~~


## 何をするのか

ビルドを半自動化、また更新日時の比較や並列処理による高速化[^1]をします。

---

## 使い方

### 基本の形
```
gonmakev2 --gmakefile ＜gmakeファイル＞ --dir ./ --outpath ＜出力する名前＞
```

### オプション解説

注意：ロングネームしかありません。

| 大事 | 名前 | 引数 | 初期値 | 説明 |
| :--- | :--- | :--- | :----- | :--- |
| **必須** | `--dir` | `<PATH>ディレクトリ` | （なし） | ビルドするディレクトリを指定します。 |
| **必須** | `--gmakefile` | `<PATH>gmakeファイル` | （なし） | ビルドする詳細のgmakeファイルを指定します。 |
|      | `--outpath` | `<PATH>出力するファイル` | `default.exe` | 出力するファイルのパスを指定します。 |
|      | `--thread` | `<UINT>スレッド数` | `2` | スレッド数を指定します |
|      | `--optionecho` | `<BOOL>`（なし） | `false` | オプションをコンソール上に出力することを指定します。 |
|      | `--buildfileecho` | `<BOOL>`（なし） | `false` | ビルドするファイルをコンソール上に出力することを指定します。 |
|      | `--buildcommandecho` | `<BOOL>`（なし） | `false` | ビルドするコマンドをコンソール上に出力することを指定します。 |
|      | `--fileserachcardsecho` | `<BOOL>`（なし） | `false` | ファイルを検索する条件をコンソール上に出力することを指定します。 |
|      | `--diff` | `<BOOL>`（なし） | `false` | ファイルと出力されたファイルの更新日時を比較してコンパイル回数を削減します。特別なオプションが必要です。 |
|      | `--demo` | `<BOOL>`（なし） | `false` | デモモードを有効化します。[^2] |
<!-- |      | `` | `` | `` |  | -->

### 発展コマンド例
`gonmakev2 --gmakefile aaa.gmake --optionecho --fileserachcardsecho --dir ./ --buildfileecho --buildcommandecho --thread 2 --diff --outpath gonmakev2_selfbuild.exe`

### gmakeファイルとは。。。

gonmakev2でのみ使用できるビルドの設定ファイル

^files、^eachbuildcommand、^linkbuildcommandの三つの項目からなる。項目の中はスペースを四つ先頭に挟まないと正しく認識されないので注意。

;でコメントできるが、必ず行の先頭、スペースを開けずに書かなければならない。

サンプルコード

```gmake
; aaa.gmake
^files
    -white
    @./src/*.cpp
^eachbuildcommand
    @clear
    ccache clang++ -c --target=x86_64-w64-mingw32 -std=c++26 -O0 -fdiagnostics-absolute-paths -fexperimental-library
    @space
    -fcolor-diagnostics -fansi-escape-codes -MMD
    @space
    @infilepath
    @space
    -o
    @space
    @outfilepath
    @space
    -MF
    @space
    @outfilepath
    .d
^linkbuildcommand
    @clear
    clang++
    @space
    @temppaths
    -o
    @outfilepath
    @space
    --target=x86_64-w64-mingw32 -fuse-ld=lld -std=c++26 -fdiagnostics-absolute-paths
```


--diffを使う場合は
clang++のオプションに-MMDを追加することと、
    -MF
    @space
    @outfilepath
    .d
が必須です。

#### ^files

ファイルを検索する条件を指定する

-whiteで追加、-blackで除外するようにモードを切り替えることができる。

`@○○`という形で指定することができ、@を除いた文字が認識される。また、ワイルドカードに対応している。このワイルドカードはファイルの相対パスを基準に作られているので、ファイル名の先頭の文字、などはできない。


#### ^eachbuildcommand

@clearで初期値をリセット、

@spaceでスペースを挟み[^3]、

@infilepathで入力ファイルのパス、

@outfilepathで出力するファイルのパスを指定できる。

そのまま描いた場合は固形テキストとして認識される。

これは各ファイルに対して実行され、それぞれ結合される。

また、@infilepathに入るのは検索した対象に引っかかったファイルで、@outfilepathはパスの処理をした名前[^4]+.oという形になる

### ^linkbuildcommand

@clearや@space、固形テキストは^eachbuildcommandの仕様と同様。

@temppathsは@outfilepathのパスをスペースで連結したもの。これは--diffが入っていても、検索に引っかかったものの@outfilepathが強制的に入る。

@outfilepathはほぼ役割は同じだが中身が変わり、中身は--outpathで指定したものになる。

---

## インストール方法

https://github.com/gonterao-crypto/gonmakev2/releases

ここから、最新版をダウンロード[^5]し、

環境変数を通すか、プロジェクトと同じディレクトリにおいて[^6]、

コマンドプロンプトなどから実行してください。

---

※一部gonmakeのREADME.mdを使いまわししています。

[^1]:並列処理にするとコンソール出力が同時に出力されるバグがあります。バグがひどい場合は--thread 1をオプションにつけると改善するかもです。
[^2]:デモモードは、実際にコンパイルを実行せずにコマンドだけを処理する機能です。`--buildcommandecho`と組み合わせて使うとよいです。
[^3]:@spaceを使わなくても書くことはできる。強制的なものではない。
[^4]:ファイルの区切り記号を_に置き換えする。.cppなどの拡張子はそのまま、のちに.oが後ろにつく。
[^5]:ない場合は自分でビルドしてください。
[^6]:おすすめは環境変数を通すやり方です。