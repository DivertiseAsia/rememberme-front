const merge = require('webpack-merge');
const common = require('./webpack.common.js');
const webpack = require('webpack');
const environments = require('./environments');

const API_SERVER = process.env.API_SERVER === 'staging'
  ? environments.staging.API_URL
  : environments.dev.API_URL;
const envVars = environments.dev;

console.log('WPDEV - will use api server:' + API_SERVER);

module.exports = merge(common, {
  mode: 'development',
  devtool: 'inline-source-map',
  plugins: [
    new webpack.DefinePlugin({
      API_SERVER: JSON.stringify(API_SERVER),
      FB_PIXEL_ID: JSON.stringify(envVars.FB_PIXEL_ID),
      GOOGLE_ANALYTICS_ID: JSON.stringify(envVars.GOOGLE_ANALYTICS_ID),
    }),
  ],
  devServer: {
    historyApiFallback: true,
    headers: {
      'Access-Control-Allow-Origin': '*'
    },
    https: false,
    contentBase: "./build",
    publicPath: '/'
  },
});