import React, { useContext} from 'react';
import {
  View
} from 'react-native';

import { Text, Card, useStyleSheet, StyleService } from '@ui-kitten/components';


import { ThemeContext } from '../../theme-context';

import NumberTicker from 'react-native-number-ticker/NumberTicker';


const CardHeader = (props, title) => (
  <View {...props}>
    <Text category='c2'>{title}</Text>
  </View>
);

const PoolGaudge = ({style, value, title, status, suffix}) => {

  const themeContext = useContext(ThemeContext);
  const styles = useStyleSheet(themedStyles);

  return (
  <Card style={[styles.card, style]} header={headerProps => CardHeader(headerProps, title)} status={status}>
    <View style={styles.cardContent}>
      <NumberTicker
        style={styles.ticker}
        number={(value/1000).toFixed(2)}
        textSize={30}
        duration={1500}
        textStyle={styles.tickerText}
      /><Text style={styles.ticketTextSuffix}>{suffix}</Text>
    </View>
  </Card>);
};

const themedStyles = StyleService.create({
  ticker: {
    paddingRight: 5,
  },
  tickerText: {
    color: 'text-basic-color',
  },
  ticketTextSuffix: {
    color: 'text-basic-color',
    fontSize: 20,
    justifyContent: 'flex-end',
  },
  cardContent: {
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'flex-end'
  },
  card: {
    flexDirection: 'column',
    flex: 1,
    marginHorizontal: 5
  },
});

export default PoolGaudge;
