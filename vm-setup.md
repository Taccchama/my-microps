VMセットアップにおける手順を以下に記載する。

## 必要なツール

- VM作成前
  - ubuntuイメージ
    - ubuntu-desktopではなくubuntu-serverを利用。
    - 基本的にはMacではx64ではなく、armを使う。
      - x64の場合はrosettaの設定を忘れずに行うこと。
  - Virtual Box
    - 仮想マシンを建てられる物であればなんでも。

- VM作成後
  - git
  - make
  - gcc
    - 今夏はbuild-essentialパッケージを入れる。

## VM作成までの手順

1. こちらからイメージをインストールする。
  
- ダウンロード元
  - https://ubuntu.com/download/server/arm
  - https://jp.ubuntu.com/download

- トラブルシューティング：https://qiita.com/miyahon/items/a4123e227d91fa773e94

2. 以下の手順に従ってVMを作成する。

- https://qiita.com/tommy1038/items/bb6470acd2147f84960e

## 開発環境設定までの手順

1. sudo apt install make

2. sudo apt install git

3. sudo apt install build-essential

4. git clone <clone元> <ローカルでのディレクトリ名>

5. git switch <branch名>

