const { spawn } = require('child_process');
const path = require('path');

exports.synthesize = (req, res) => {
    const text = req.body.text;

    if (!text) {
        return res.status(400).send('テキストが必要です');
    }

    // MyTalk 実行ファイルへのパス
    const myTalkPath = path.join(__dirname, '..', 'MyTalk');

    const child = spawn(myTalkPath);

    let wavData = Buffer.alloc(0);
    let errorData = '';

    child.stdout.on('data', (data) => {
        wavData = Buffer.concat([wavData, data]);
    });

    child.stderr.on('data', (data) => {
        errorData += data;
    });

    child.on('close', (code) => {
        if (code !== 0) {
            console.error('MyTalk exited with code', code);
            console.error('Error:', errorData);
            return res.status(500).send('音声合成中にエラーが発生しました');
        }

        res.set('Content-Type', 'audio/wav');
        res.send(wavData);
    });

    // 標準入力にテキストを渡す
    child.stdin.write(text);
    child.stdin.end();
};
