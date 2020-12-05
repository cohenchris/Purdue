# CS 422 Lab 2 - Transport Protocols

## ALTERNATING BIT
- To simulate an Alternating-Bit-Protocol transport layer, execute:
      `make alt`      or simply       `make`
- NOTES
  - I used TIMEOUT = 80
  - I chose to not use NAK for this part. Instead, I simply ignore the packet that would have cause a NAK to be sent.
  - If side B receives a corrupted packet, it ignores it.

## GO-BACK-N
- To simulate a Go-Back-N transport layer, execute:
      `make gbn`
- NOTES
  - I used TIMEOUT = 20
  - Send 75 messages until 20 are received
  - I chose to not use NAK for this part. Instead, I simply ignore the packet that would have cause a NAK to be sent.
  - Every time that B receives an invalid packet, it sends an ACK for the highest seq# packet received (this is in the slides).
  - When A receives the message from the upper layer, I put a null character at the end to make it more readable. This makes the messages 19 characters instead of 20 (MESSAGE_LEN).

## SELECTIVE REPEAT
- To simulate a Selective Repeat transport layer, execute:
      `make sr`
- NOTES
  - I used TIMEOUT = 20
  - Send 45 messages until 20 are received
  - I chose to not use NAK for this part. Instead, I simply ignore the packet that would have cause a NAK to be sent.
  - When A receives the message from the upper layer, I put a null character at the end to make it more readable. This makes the messages 19 characters instead of 20 (MESSAGE_LEN).
