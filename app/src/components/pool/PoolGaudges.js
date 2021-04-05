import { StyleSheet, View } from "react-native";
import React from "react";

import PoolGaudge from './PoolGaudge';

const PoolGaudges = ({style, statsData, layoutWidth}) => (
  <View style={[styles.statsContainer, style]}>
    <PoolGaudge 
      style={{
        marginLeft: 0
      }}
      value={statsData.hash}
      title="Raw Hashrate"
      status="info"
      suffix="KH/s"
    />
    <PoolGaudge 
      value={statsData.hash2}
      title="Pay Hashrate"
      status="info"
      suffix="KH/s"
    />
    <PoolGaudge 
      value={statsData.validShares/1000}
      title="Valid Shares"
      status="success"
      suffix="M"
    />
    <PoolGaudge 
      value={statsData.invalidShares}
      title="Invalid Shares"
      status="warning"
      suffix="K"
    />
    <PoolGaudge 
      style={{
        marginRight: 0
      }}
      value={statsData.totalHashes/1000000}
      title="Total Hashes"
      status="warning"
      suffix="GH"
    />
  </View>
  
);

const styles = StyleSheet.create({
  statsContainer: {
    flexDirection: 'row',
  }
});

export default PoolGaudges;
