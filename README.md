In this project I created a stopwatch program using AVR with Timer1, INT0, INT1, INT2 and 6 seven-segments.
The stop watch counts normally and when INT0 fires, the stopwatch is reset.
When INT1 fires the counter is paused by disabling Timer1.
When INT2 fires the counter is resumed by enabling Timer1.
