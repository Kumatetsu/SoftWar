#include <czmq.h>
#include <stdio.h>

int main()
{
  zctx_t *context = zctx_new();
  void *responder = zsocket_new(context, ZMQ_REP);
  zsocket_bind(responder, "tcp://*5555");

  while (!zctx_interrupted)
    {
      char *message = zstr_recv(responder);
      printf("Received %s\n", message);
      sleep(1);
      zstr_sendf(responder, "%s", message);
      free(message);
    }
  zctx_destroy(&context);
  return(0);
}
