FROM node:14

# ビルドツールのインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリの設定
WORKDIR /usr/src/app

# AquesTalk2 評価版のダウンロードとインストール
RUN wget http://www.a-quest.com/download/package/aqtk2-lnx-eva_230.zip \
    && unzip aqtk2-lnx-eva_230.zip \
    && cp aqtk2-lnx-eva_230/lib64/libAquesTalk2Eva.so.2.3 /usr/lib64/ \
    && ln -sf /usr/lib64/libAquesTalk2Eva.so.2.3 /usr/lib64/libAquesTalk2Eva.so.2 \
    && ln -sf /usr/lib64/libAquesTalk2Eva.so.2 /usr/lib64/libAquesTalk2Eva.so \
    && ldconfig \
    && cp aqtk2-lnx-eva_230/lib64/AquesTalk2.h /usr/src/app/ \
    && cp aqtk2-lnx-eva_230/phont/aq_yukkuri.phont /usr/src/app/

# MyTalk.c のコピー
COPY MyTalk.c /usr/src/app/

# MyTalk のコンパイル
RUN gcc -o MyTalk MyTalk.c -lAquesTalk2Eva

# package.json のコピーと依存関係のインストール
COPY package*.json /usr/src/app/
RUN npm install

# アプリケーションコードのコピー
COPY . /usr/src/app

# ポートの公開
EXPOSE 8080

# アプリケーションの起動
CMD ["npm", "start"]