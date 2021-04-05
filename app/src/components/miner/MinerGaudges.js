import { StyleSheet, View } from "react-native";
import React from "react";

import MinerGaudge from './MinerGaudge';

const MinerGaudges = ({style, statsData, layoutWidth}) => (
  <View style={[styles.statsContainer, style]}>
    <MinerGaudge 
      value={statsData.last10s}
      maxValue={statsData.top} 
      layoutWidth={layoutWidth}
      title="Last 10 sec"
    />
    <MinerGaudge 
      style={styles.centerGaudge}
      value={statsData.last60s}
      maxValue={statsData.top} 
      layoutWidth={layoutWidth}
      title="Last 60 sec"
    />
    <MinerGaudge 
      value={statsData.last15m}
      maxValue={statsData.top} 
      layoutWidth={layoutWidth}
      title="Last 15 min"
    />
  </View>
);

const styles = StyleSheet.create({
  statsContainer: {
    flexDirection: 'row',
    padding: 0,
    margin: 0,
  },
  centerGaudge: {
    marginHorizontal: 10
  }
});

export default MinerGaudges;
