# VESC Canbus Communication documentation

 - A CAN Bus frame can contain a maximum of 8 bytes of data.
 - Each CAN node has a unique ID, the ID determines the priority (ID 0 is most dominant).
 - CAN messages from VESC use an extended ID (EID),  containing the COMMAND and CONTROLLER_ID.

## CANBus Control

These commands can be sent to a VESC node to control the motor and request status.

| Command                          | ID | Data                | Data Length   | Data Type      | Units |
|----------------------------------|----|---------------------|---------------|----------------|-------|
| CAN_PACKET_SET_DUTY              | 0  | Motor Duty Cycle    | 32-bit/4-byte | Signed Integer | Thousandths of percent (50000 --> 50%)     |
| CAN_PACKET_SET_CURRENT           | 1  | Motor Current       | 32-bit/4-byte | Signed Integer | mAh   |
| CAN_PACKET_SET_CURRENT_BRAKE     | 2  | Motor Brake Current | 32-bit/4-byte | Signed Integer | mAh   |
| CAN_PACKET_SET_RPM               | 3  | Motor RPM           | 32-bit/4-byte | Signed Integer  | ERPM  |
| CAN_PACKET_SET_POS               | 4  | Motor Position
| CAN_PACKET_FILL_RX_BUFFER        | 5  | | |
| CAN_PACKET_FILL_RX_BUFFER_LONG   | 6  | | |
| CAN_PACKET_PROCESS_RX_BUFFER     | 7  | | |
| CAN_PACKET_PROCESS_SHORT_BUFFER  | 8  | | |
| CAN_PACKET_STATUS                | 9  | Request status | N/A |
| CAN_PACKET_SET_CURRENT_REL       | 10 | | |
| CAN_PACKET_SET_CURRENT_BRAKE_REL | 11 | | |

* Motor position requires an encoder to be present.
