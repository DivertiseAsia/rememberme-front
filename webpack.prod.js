const path = require('path');
const merge = require('webpack-merge');
const common = require('./webpack.common.js');
const webpack = require('webpack');
const outputDir = path.join(__dirname, "build/");

const HtmlWebpackPlugin = require('html-webpack-plugin');
const HtmlWebpackExcludeAssetsPlugin = require('html-webpack-exclude-assets-plugin');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CopyWebpackPlugin = require('copy-webpack-plugin');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const environments = require('./environments');

const envVars = process.env.API_SERVER === 'staging'
  ? environments.staging
  : environments.prod;

console.log('WPProd - will use api server:' + envVars.API_URL);

module.exports = merge(common, {
  mode: 'production',
  output: {
    path: outputDir,
    publicPath: '/',
    filename: '[name].js'.toLowerCase(),
  },
  plugins: [
    new webpack.DefinePlugin({
      API_SERVER: JSON.stringify(envVars.API_URL),
      FB_PIXEL_ID: JSON.stringify(envVars.FB_PIXEL_ID),
      GOOGLE_ANALYTICS_ID: JSON.stringify(envVars.GOOGLE_ANALYTICS_ID),
    }),
    new CleanWebpackPlugin([outputDir]),
    new HtmlWebpackPlugin({
      filename: 'index.html',
      template: 'src/index.html',
    }),
    new HtmlWebpackExcludeAssetsPlugin(),
    new MiniCssExtractPlugin({
      filename: '[name].[hash].css',
      chunkFilename: '[id].[hash].css',
    }),
    new CopyWebpackPlugin([
      { from: 'public' }
    ]),
  ]
});