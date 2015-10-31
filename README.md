# tcp_nonblocking_server
Simple tcp_nonblocking server


1_Error: Resource temporarily unavailable(EAGAIN)
=>the operation would have blocked but nonblocking operation was requested

   When the message does not fit into  the  send  buffer  of  the  socket,
   send() normally blocks, unless the socket has been placed in non-block-
   ing I/O mode.  In non-blocking mode it  would  return  EAGAIN  in  this
   case.  
