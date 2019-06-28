const environments = {
  dev:
  {
    ENV_NAME: 'dev',
    FB_PIXEL_ID: '', // empty => no tracking
    GOOGLE_ANALYTICS_ID: '', // empty => no tracking
    API_URL: 'http://192.168.99.100:8000/',
  },
  staging:
  {
    ENV_NAME: 'staging',
    FB_PIXEL_ID: '', // empty => no tracking
    GOOGLE_ANALYTICS_ID: '', // empty => no tracking
    API_URL: 'https://api.staging.artcrm.com/',
  },
  prod:
  {
    ENV_NAME: 'production',
    FB_PIXEL_ID: '', // empty => no tracking
    GOOGLE_ANALYTICS_ID: '', // empty => no tracking
    API_URL: 'https://api.artcrm.com/',
  },
};

module.exports = environments;
