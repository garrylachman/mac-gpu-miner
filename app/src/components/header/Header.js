import { StyleSheet, View } from "react-native";
import React, { useContext} from "react";
import {
    Text,
    Toggle
} from '@ui-kitten/components';
import { ThemeContext } from '../../theme-context';

const Header = () => {

    const themeContext = useContext(ThemeContext);

    return (
        <View style={styles.view}>
            <Text style={styles.text}>MAC GPU MINER</Text>
            <Toggle checked={themeContext.theme == 'light'} onChange={themeContext.toggleTheme}>
            </Toggle>
        </View>
    );
};

const styles = StyleSheet.create({
  text: {
    fontSize: 30,
    fontWeight: "600",
    textAlign: "left",
    color: "#61dafb"
  },
  view: {
    backgroundColor: "#20232a",
    padding: 10,
    flexDirection: 'row',
    justifyContent: 'space-between'
  }
});

export default Header;