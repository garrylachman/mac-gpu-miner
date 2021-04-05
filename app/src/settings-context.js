import React from 'react';

const SettingsContextDefaultSettings = {
  theme: 'light',
  wallet: undefined
};

const SettingsContext = React.createContext({
  settings: undefined,
  setSetting: (key, value) => {},
});

export {
  SettingsContextDefaultSettings,
  SettingsContext
};