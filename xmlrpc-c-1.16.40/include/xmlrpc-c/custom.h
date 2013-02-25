typedef struct sem_type{
  int semantic_type;
  int count;
  int flag;
  int state; //1 = READY 0 = DONE
  int num_of_servers;
  int num_of_requests;
  int rpc_fail_count;
  int temp_head_count;
  int temp_tail_count;
  int return_value;
} semantic_t;

semantic_t temp;
