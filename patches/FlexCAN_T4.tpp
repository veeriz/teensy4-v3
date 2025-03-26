diff --git a/FlexCAN_T4.tpp b/FlexCAN_T4.tpp
index 1346..fixed b/FlexCAN_T4.tpp
--- a/FlexCAN_T4.tpp
+++ b/FlexCAN_T4.tpp
@@ -1343,7 +1343,8 @@ bool FlexCAN_T4<_bus, _rxSize, _txSize>::error(CAN_error_t& err, bool printDetails
   if ( (error.ESR1 & 0x30) == 0x0 ) strncpy((char*)error.FLT_CONF, "Error Active", (sizeof(error.FLT_CONF)));
-  else if ( (error.ESR1 & 0x30) == 0x1 ) strncpy((char*)error.FLT_CONF, "Error Passive", (sizeof(error.FLT_CONF) - 1));
+  else if ( (error.ESR1 & 0x30) == 0x1 ) {
+    strncpy((char*)error.FLT_CONF, "Error Passive", sizeof(error.FLT_CONF) - 1);
+    error.FLT_CONF[sizeof(error.FLT_CONF) - 1] = '\0';
+  }