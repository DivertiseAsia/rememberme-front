const path = require('path');
const { merge } = require('webpack-merge');
const common = require('./webpack.common.js');
const webpack = require('webpack');
const outputDir = path.join(__dirname, "build/");

const HtmlWebpackPlugin = require('html-webpack-plugin');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CopyWebpackPlugin = require('copy-webpack-plugin');
const environments = require('./environments');

const envVars = process.env.API_SERVER === 'staging'
  ? environments.staging
  : environments.prod;

const ignoreFiles = process.env.API_SERVER === 'staging'
  ? []
  : [/*'robots.txt' when ready to go live */];
console.log('WPProd - will ignore these files:' + ignoreFiles);
console.log('WPProd - will use api server:' + envVars.API_URL);

module.exports = merge(common, {
  mode: 'production',
  plugins: [
    new webpack.DefinePlugin({
      API_SERVER: JSON.stringify(envVars.API_URL),
      APP_URL: JSON.stringify(envVars.APP_URL),
      FB_PIXEL_ID: JSON.stringify(envVars.FB_PIXEL_ID),
      GOOGLE_ANALYTICS_ID: JSON.stringify(envVars.GOOGLE_ANALYTICS_ID),
    }),
    new HtmlWebpackPlugin({
      filename: 'index.html',
      template: 'src/index.html',
    }),
    new MiniCssExtractPlugin({
      filename: '[name].[contenthash].css',
      chunkFilename: '[id].[contenthash].css',
    }),
    new CopyWebpackPlugin({
      patterns: [
        {
          from: 'public',
          globOptions: {
            ignore: ignoreFiles
          }
        }
      ]
    }),
  ],
  optimization: {
    runtimeChunk: 'single',
    splitChunks: {
      chunks: 'all'
    }
  }
});