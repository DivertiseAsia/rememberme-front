const path = require('path');
const GitRevisionPlugin = require('git-revision-webpack-plugin');
const webpack = require('webpack');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const HtmlWebpackExcludeAssetsPlugin = require('html-webpack-exclude-assets-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const gitRevisionPlugin = new GitRevisionPlugin({ branch: true });
const outputDir = path.join(__dirname, "build/");

let versioning = null;
try {
  versioning = JSON.stringify({
    VERSION: gitRevisionPlugin.version(),
    COMMITHASH: gitRevisionPlugin.commithash(),
    BRANCH: gitRevisionPlugin.branch()
  });
} catch (e) {
  versioning = {
    VERSION: 'n/a',
    COMMITHASH: '000000',
    BRANCH: 'n/a'
  }
}

module.exports = {
  entry: {
    'index': './lib/js/src/Index.bs.js',
    'style': path.join(__dirname, 'public', 'css', 'style.scss'),
    'sw': "./lib/js/src/serviceWorker/ServiceWorkerListener.bs.js",
  },
  output: {
    path: outputDir,
    publicPath: '/',
    filename: '[name].js'.toLowerCase(),
  },
  stats: 'minimal',
  plugins: [
    new webpack.DefinePlugin({
      BUILD: versioning
    }),
    new HtmlWebpackPlugin({
      filename: 'index.html',
      template: 'src/index.html',
      excludeAssets: [/style.js/]
    }),
    new MiniCssExtractPlugin({
      filename: '[name].[hash].css',
      chunkFilename: '[id].[hash].css',
    }),
    new HtmlWebpackExcludeAssetsPlugin(),
    new CopyWebpackPlugin([
      { from: 'public' },
      { from: 'public/images/favicon.png' }
    ]),
  ],
  module: {
    rules: [{
      test: /\.(jpe?g|png|gif|svg|pdf|ico)$/,
      use: [{
        loader: 'file-loader',
        options: {
          name: '[path][name]-[hash:6].[ext]'
        },
      },]
    },
    {
      use: 'babel-loader',
      test: /\.js$/,
      exclude: /node_modules/,
    },
    {
      test: /\.scss$/,
      use: [
        MiniCssExtractPlugin.loader,
        {
          loader: "css-loader" // translates CSS into CommonJS
        },
        {
          loader: "sass-loader" // compiles Sass to CSS
        }
      ]
    }
    ]
  },
};
