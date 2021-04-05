import { StyleSheet, View, SafeAreaView } from "react-native";
import React, { useState, useContext, useReducer, useLayoutEffect } from "react";

import { 
    Text,
    Button,
    Input
} from '@ui-kitten/components';

import { SettingsContext } from '../../settings-context';

import { Pulse } from 'react-native-animated-spinkit'

const validateWalletAddress = (addr) => {
    return addr != null && /[48][0-9AB][1-9A-HJ-NP-Za-km-z]{93}/.test(addr);
}

const SideBar = ({startJob, isMining=false}) => {
    const settingsContext = useContext(SettingsContext);

    const [ignored, forceUpdate] = useReducer(x => x + 1, 0);

    const [ wallet, setWallet ] = useState(settingsContext.settings.wallet);
    const [ isWalletValid, setIsWalletValid ] = useState(false);

    const LoadingIndicator = (props) => (
        <>
        { isMining && 
        <Pulse {...props} size={20} animating="true" color="#FFF" />}
        </>
      );
    
    useLayoutEffect(() => {
        setIsWalletValid(validateWalletAddress(wallet));
        settingsContext.setSetting('wallet', wallet);
    }, [wallet]);

    return (
        <SafeAreaView>
            
                <Text category='h4'>Let's Start</Text>
                <Text style={styles.text} appearance='hint'>It's easy, we done all the hard work. Just enter your Monero Wallet address and press start.</Text>
                <Text style={styles.text} appearance='hint'>We use MoneroOcean pool and mine CN/GPU coins, the pool is payout in XMR. The miner and the config built specific for Mac computers that got AMD Radeon GPU.</Text>

                <Input
                    caption={!isWalletValid ? 'Error: XMR address is not valid.' : 'Valid XMR address.' }
                    label="XMR Wallet Address"
                    style={styles.input}
                    status={isWalletValid ? 'success' : 'danger' }
                    placeholder='888tNkZrPN6JsEgekjMnABU4TBzc2Dt29EPAvkRxbANsAnjyPbb3iQ1YBRk1UXcdRsiKc9dhwMVgN5S9cQUiyoogDavup3H'
                    onChangeText={val => setWallet(val)}
                    value={wallet}
                    disabled={isMining}
                />
                
                <View style={styles.buttonsRow}>
                    <Button style={styles.button} onPress={() => startJob({'wallet': wallet})} status="primary" disabled={!isWalletValid || isMining} accessoryLeft={LoadingIndicator}>Start</Button>
                    <Button style={styles.button} onPress={() => startJob({})} status="danger" disabled={!isMining}>Stop</Button>
                </View>
        </SafeAreaView>
    );
};

const styles = StyleSheet.create({
    text: {
        paddingTop: 5
    },
    input: {
        flex: 1,
        paddingTop: 20
    },
    button: {
        margin: 2
    },
    buttonsRow: {
        flexDirection: 'row',
        flexWrap: 'wrap'
    },
    indicator: {
  
    },
    container: {
        flexDirection: 'column',
        flexWrap: 'nowrap'
      }
});

export default SideBar;
