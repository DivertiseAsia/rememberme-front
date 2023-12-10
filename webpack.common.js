const path = require('path');
const { GitRevisionPlugin } = require('git-revision-webpack-plugin');
const webpack = require('webpack');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CopyWebpackPlugin = require('copy-webpack-plugin');
const gitRevisionPlugin = new GitRevisionPlugin({ branch: true });
const outputDir = path.join(__dirname, "build/");

let versioning = null;

const isMobile = process.env.MOBILE_BUILD || false;
let publicPath = isMobile ? '' : '/';
if (process.env.PUBLIC_PATH) {
  publicPath = process.env.PUBLIC_PATH;
}
console.log(`Public path for app will be: "${publicPath}"`);

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
    'style': path.join(__dirname, 'public', 'css', 'style.scss')
  },
  output: {
    path: outputDir,
    publicPath,
    filename: '[name].[chunkhash].js'.toLowerCase(),
    chunkFilename: '[name].[chunkhash].chunk.js'.toLowerCase(),
    clean: true
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
      filename: '[name].[contenthash].css',
      chunkFilename: '[id].[contenthash].css',
    }),
    new CopyWebpackPlugin({
      patterns: [
        { from: 'public/images/favicon.png' },
      ]
    }),
  ],
  module: {
    rules: [{
      test: /\.(jpe?g|png|gif|svg|pdf|ico)$/,
      type: 'asset/resource',
      generator: {
        filename: 'static/[hash][ext][query]'
      }
    },
    {
      use: 'babel-loader',
      test: /\.js$/,
      exclude: /node_modules\/(?!react-intl|intl-messageformat|@formatjs\/icu-messageformat-parser)/,
    },
    {
      test: /\.json$/,
      type: 'json',
    },
    {
      test: /\.css$/,
      use: ['style-loader', 'css-loader'],
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
