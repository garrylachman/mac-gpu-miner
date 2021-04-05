/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow strict-local
 */

import React, { useState, useEffect, useRef } from 'react';
import {
  StyleSheet,
  NativeModules,
  NativeEventEmitter,
  View,
  Settings,
  FlatList
} from 'react-native';

import * as eva from '@eva-design/eva';
import { ApplicationProvider, Layout, Text, IconRegistry } from '@ui-kitten/components';
import { EvaIconsPack } from '@ui-kitten/eva-icons';

import { ThemeContext } from './src/theme-context';
import { SettingsContext, SettingsContextDefaultSettings } from './src/settings-context';

import {
  Header,
  SideBar,
  MinerGaudges,
  PoolGaudges
} from './src/components';

const { MinerModule } = NativeModules;


const CardHeader = (props, title) => (
  <View {...props}>
    <Text category='c2'>{title}</Text>
  </View>
);

const regex = /^\[([0-9\- \:]+)\]\s:\s(.*)$/m;

const App = ({eva}) => {
  const [job, setJob] = useState({});
  const [minerStats, setMinerStats] = useState({ 'last10s':0, 'last60s': 0, 'last15m': 0, 'top': 100});
  const [poolStats, setPoolStats] = useState({ amtDue: 0, amtPaid: 0, hash: 0, hash2: 0, invalidShares: 0, lastHash: 0, totalHashes: 0, txnCount: 0, validShares: 0});
  const [minerLog, setMinerLog] = useState([]);
  
  const [mainViewLayout, setMainViewLayout] = useState({
    left: 0,
    top: 0,
    width: 0,
    height: 0,
  })

  const logListRef = useRef(null);


  useEffect(() => {
    const MinerEmitter = new NativeEventEmitter(MinerModule);

    if (job != null && job['wallet'] != null) {

      

      MinerModule.bindMinerEvents();
      
      MinerModule.setWallet(job['wallet']);
      
      MinerModule.startMiner();    

      MinerEmitter.addListener('hashrate', (data) => {        
        setMinerStats(data);
      });

      MinerEmitter.addListener('pool', (data) => {
        setPoolStats(data);
      })

      MinerEmitter.addListener('miner_log', (data) => {
        console.log(data);
        const [date, value] = data.split(/\[([0-9\- \:]+)\]\s:\s(.*)/m).filter(v => v != "");
        setMinerLog(prevState => [...prevState, {date: date, value: value, key: `${Math.round(Math.random()*10000000)}`}]);
      })

    } else {
      MinerModule.stopMiner();
      MinerEmitter.removeAllListeners('hashrate');
      MinerEmitter.removeAllListeners('pool');
      MinerEmitter.removeAllListeners('miner_log');
    }
  }, [job]);

  const renderLogItem = (info) => {
    
    let itemStatus = '';
    if (/accepted|connected|logged/i.test(String(info.item.value))) {
      itemStatus = 'success';
    }
    if (/error|rejected|failed|dead|exit/i.test(String(info.item.value))) {
      itemStatus = 'danger';
    }
    if (/warning|timeout/i.test(String(info.item.value))) {
      itemStatus = 'warning';
    }
    if (/difficulty|new block|connecting|compiling|starting/i.test(String(info.item.value))) {
      itemStatus = 'info';
    }
    
    return (<View style={styles.logItem}>
      <Text category='label' style={{paddingRight: 5}}>{info.item.date}</Text>
      <Text category='c2' status={itemStatus}>{info.item.value}</Text>
    </View>);
  }

  
  return (
    <>      
      <Header />
      <Layout style={styles.container}>
        <Layout style={[(job != null && job['wallet'] != null) ? styles.sideLayoutHalf : styles.sideLayoutFull]} level='2'>
          <SideBar startJob={(data) => setJob(data)} isMining={job != null && job['wallet']} />
        </Layout>

        {(job != null && job['wallet'] != null) &&
        <Layout onLayout={(event) => setMainViewLayout(event.nativeEvent.layout)} style={styles.mainLayout} level='1'>
          <Text style={{paddingBottom: 10}} category='h2'>Realtime Miner Stats</Text>

          <MinerGaudges
            style={styles.widgetRow}
            statsData={minerStats}
            layoutWidth={mainViewLayout.width}
          />

          <Text style={{paddingVertical: 10}} category='h2'>Your MoneroOcean Pool Stats</Text>
          
          
          <PoolGaudges
            style={styles.widgetRow}
            statsData={poolStats}
            layoutWidth={mainViewLayout.width}
          />

          <Text style={{paddingVertical: 10}} category='h3'>Miner Backend Realtime Log</Text>

          <View style={styles.logContainer}>
            <FlatList
              ref={logListRef}
              onContentSizeChange={()=> logListRef.current.scrollToEnd()}
              data={minerLog}
              renderItem={renderLogItem}
            />
          </View>
    
          
        </Layout>}
      </Layout>
    </>
  );
};


const styles = StyleSheet.create({
  logContainer: {
    width: '100%',
    flexDirection: 'row',
    flex: 1
  },
  logContentContainer: {
    paddingHorizontal: 0,
    paddingVertical: 0,
  },
  logItem: {
    marginVertical: 0,
    flexDirection: 'row',
  },
  titleText: {
  },
  text: {
    fontSize: 20,
    color: '#000'
  },
  engine: {
    position: 'absolute',
    right: 0,
  },
  highlight: {
    fontWeight: '700',
  },
  footer: {
    fontSize: 12,
    fontWeight: '600',
    padding: 4,
    paddingRight: 12,
    textAlign: 'right',
  },
  container: {
    flex: 1,
    flexDirection: 'row',
  },
  sideLayoutHalf: {
    flex: 0.3,
    padding: 10,
    justifyContent: 'flex-start',
    alignItems: 'flex-start',
  },
  sideLayoutFull: {
    flex: 1,
    padding: 10, 
    justifyContent: 'flex-start',
    alignItems: 'flex-start',
  },
  mainLayout: {
    flex: 0.7,
    justifyContent: 'flex-start',
    alignItems: 'center',
    padding: 10
  },
});

export default () => {
  
  const [settings, setSettings] = useState(
    Settings.get('data') ? 
      {...SettingsContextDefaultSettings, ...Settings.get('data')} : 
      {...SettingsContextDefaultSettings}
    );
  const setSetting = (key, value) => {
    const newObj = {...settings, ...{[`${key}`]: value}};
    Settings.set({
      data: newObj
    });
    setSettings(newObj);
  };

  const [theme, setTheme] = useState(settings.theme);

  const toggleTheme = () => {
    const nextTheme = theme === 'light' ? 'dark' : 'light';
    setTheme(nextTheme);
    setSetting('theme', nextTheme);
  };


  return (
    <>
      <IconRegistry icons={EvaIconsPack}/>
      <SettingsContext.Provider value={{ settings, setSetting }}>
        <ThemeContext.Provider value={{ theme, toggleTheme }}>
          <ApplicationProvider {...eva} theme={eva[theme]}>
            <App eva={eva} />
          </ApplicationProvider>
        </ThemeContext.Provider>
      </SettingsContext.Provider>
    </>
  );
};