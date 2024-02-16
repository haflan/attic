const glob = require('glob');
const path = require('path');
const VueLoaderPlugin = require('vue-loader/lib/plugin');

module.exports = {
    entry: "./src/app.js",
    output: {
        path: path.resolve(__dirname, "./dist/"),
        filename: "app.js"
    },
    module: {
        rules: [
            {test: /\.js$/, use: 'babel-loader'},
            {test: /\.vue$/, use: 'vue-loader'},
            {test: /\.css$/, use: ['vue-style-loader', 'css-loader']},
        ]
    },
    plugins:[
        new VueLoaderPlugin()
    ]
};
