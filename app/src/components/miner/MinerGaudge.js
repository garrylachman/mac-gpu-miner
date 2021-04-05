import React, { useContext, useCallback } from 'react';
import {
  View
} from 'react-native';

import { Text, Card, useStyleSheet, StyleService } from '@ui-kitten/components';

import AnimatedSpeedometer from 'react-native-cool-speedometer';

import { ThemeContext } from '../../theme-context';

import ChromaStatic from 'chroma-js';


const CardHeader = (props, title) => (
  <View {...props}>
    <Text category='c2'>{title}</Text>
  </View>
);

const MinerGaudge = ({style, value, maxValue, title, layoutWidth}) => {
  
  const themeContext = useContext(ThemeContext);
  const styles = useStyleSheet(themedStyles);

  const colorScale = useCallback(
    ChromaStatic.scale([
      styles.arcColorLow.color,
      styles.arcColorMiddle.color,
      styles.arcColorGood.color
    ]).mode('lab')
    , [themeContext.theme]);

  return (
  <Card style={[styles.card, style]} header={headerProps => CardHeader(headerProps, title)} status='info'>
      <AnimatedSpeedometer 
        style={styles.speedometer}
        size={((layoutWidth)/3)-64}
        value={value}
        max={maxValue} 
        backgroundAngle={180} 
        angle={160} 
        step={maxValue/6}
        primaryArcColor={colorScale(value/maxValue).toString()}
        primaryArcWidth={5}
        backgroundColor={styles.background.color}
        accentColor={styles.accent.color}
        needle={styles.needle}
        marks={{
          lineColor: styles.marks.color,
          numbersRadius: 20,
          numbersFontSize: 15,
          roundNumbers: true
        }}
        indicatorStyle={styles.indicator}
        indicatorSuffixStyle={styles.indicatorSuffix}
        indicatorSuffix=' KH/s'
        calcSizeByAngle={true}
        calcSizeByAngleIndicatorHeight={80}
      />
    </Card>);
};

const themedStyles = StyleService.create({
  needle: {
    color: 'color-basic-300'
  },
  marks: {
    color: 'color-basic-400'
  },
  accent: {
    color: 'color-basic-800'
  },
  background: {
    color: 'color-basic-1000'
  },
  arcColorLow: {
    color: 'color-danger-500'
  },
  arcColorMiddle: {
    color: 'color-warning-500'
  },
  arcColorGood: {
    color: 'color-success-500'
  },
  speedometer: {
    marginRight: 0,
    alignItems: 'center',
    margin: 0,
    padding: 0
  },
  indicator: {
    position: 'relative',
    fontSize: 30,
    color: 'text-basic-color',
    padding: 0
  },
  indicatorSuffix: {
    position: 'relative',
    fontSize: 20,
    color: 'text-basic-color'
  },
  card: {
    flexDirection: 'column',
    flex: 1
  }
});

export default MinerGaudge;
