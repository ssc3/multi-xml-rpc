

#include <stdlib.h>
#include <stdio.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#include "config.h"  /* information about this build environment */
#include "xmlrpc-c/custom.h"
#include "time.h"

#define NAME "Xmlrpc-c Asynchronous Test Client"
#define VERSION "1.0"

int num_of_servers = 0; //number of servers
int num_of_requests = 0;
struct timeval start, end;
unsigned long long average_time = 0;


static void 
die_if_fault_occurred (xmlrpc_env *env) {
    if (env->fault_occurred) {
        fprintf(stderr, "Something failed. %s (XML-RPC fault code %d)\n",
                env->fault_string, env->fault_code);
        exit(1);
    }
}
stats_t statistic[30];
sync_t sync_temp;

void
print_stats(int i, int sync_async, unsigned long long average_time)
{
  printf("%d|%d|%d|%d|%d|%lld\n", i+1, statistic[i].head_count, statistic[i].tail_count, statistic[i].rpc_fail_count, sync_async, average_time);


}

void
clear_all_stats()
{
  int i;
  for (i = 0; i < 3; i++)
  {
    statistic[i].semantic_type = 0;
    statistic[i].head_count = 0;
    statistic[i].tail_count = 0;
    statistic[i].rpc_fail_count = 0;
    statistic[i].sync_async = 0;
    statistic[i].avg_time = 0;
  }
}

void
xmlrpc_client_call_asynch_temp(const char ** const url,
                          int num_of_servers,
			  int adder,
                          const char * const methodName,	
                          xmlrpc_response_handler handle,
                          void * const userData,
                          const char * const format,
                          ...) {

    int i;
    int iter_num_of_requests = 0;

    temp.num_of_requests = num_of_requests;
    temp.num_of_servers = num_of_servers;

    int trick_count = 0;

    while (trick_count <5 )
    {
      for (iter_num_of_requests = 0; iter_num_of_requests < num_of_requests/5; iter_num_of_requests++)
      {
        temp.state = 1;	
        temp.temp_head_count = 0;
        temp.temp_tail_count = 0;
        temp.count = 0;
        temp.flag = 0;

        for (i=0; i<num_of_servers; i++)
        {
          xmlrpc_client_call_asynch(url[i], methodName,
                                       handle, NULL,
                                      "(ii)", (xmlrpc_int32) 5, adder);
        }
      }
      xmlrpc_client_event_loop_finish_asynch();
      trick_count++;
    } //trick_count

      for (iter_num_of_requests = 0; iter_num_of_requests < num_of_requests%5; iter_num_of_requests++)
      {
        temp.state = 1;	
        temp.temp_head_count = 0;
        temp.temp_tail_count = 0;
        temp.count = 0;
        temp.flag = 0;

        for (i=0; i<num_of_servers; i++)
        {
          xmlrpc_client_call_asynch(url[i], methodName,
                                       handle, NULL,
                                      "(ii)", (xmlrpc_int32) 5, adder);
        }
      }
      xmlrpc_client_event_loop_finish_asynch();
}

void
handle(const char *   const server_url,
                           const char *   const method_name,
                           xmlrpc_value * const param_array,
                           void *         const user_data ATTR_UNUSED,
                           xmlrpc_env *   const faultP,
                           xmlrpc_value * const resultP) {
    
    xmlrpc_env env;
    xmlrpc_int addend, adder;

    
    /* Initialize our error environment variable */
    xmlrpc_env_init(&env);

    /* Our first four arguments provide helpful context.  Let's grab the
       addends from our parameter array. 
    */
    xmlrpc_decompose_value(&env, param_array, "(ii)", &addend, &adder);
    die_if_fault_occurred(&env);

    //printf("RPC with method '%s' at URL '%s' to add %d and %d "
           //"has completed\n", method_name, server_url, addend, adder);
    
    if (faultP->fault_occurred)
        printf("The RPC failed.  %s", faultP->fault_string);
    else {
        xmlrpc_int sum;

        xmlrpc_read_int(&env, resultP, &sum);
        die_if_fault_occurred(&env);
        //printf("In callback function \n");
	//print_stats(temp);
        if(temp.return_value == 1)
        statistic[temp.semantic_type].head_count++;
        else if(temp.return_value == 0)
        statistic[temp.semantic_type].tail_count++;
        else if(temp.return_value == 2)
        statistic[temp.semantic_type].rpc_fail_count++;   

        /*Reset all temp values here*/
        if(temp.semantic_type == 0)
        temp.count = 0;
 
	temp.flag = 0;
        temp.return_value = 0;
        temp.temp_head_count = 0;
        temp.temp_tail_count = 0;    
        temp.state = 0;

    }
}



int 
main(int           const argc, 
     const char ** argv ATTR_UNUSED) {


    //int count = 3;	
    num_of_requests = atoi(argv[1]);
    num_of_servers = atoi(argv[2]);

    char * url[num_of_servers];
    int iter_semantic = 0;
    //printf("Num_requests = %d, num_servers = %d\n", num_of_requests, num_of_servers);
    

    url[0]= "http://localhost:2000/RPC2";
    url[1]= "http://localhost:2001/RPC2";
    url[2]= "http://localhost:2002/RPC2";
    url[3]= "http://localhost:2003/RPC2";
    url[4]= "http://localhost:2004/RPC2";
    url[5]= "http://localhost:2005/RPC2";
    url[6]= "http://localhost:2006/RPC2";
    url[7]= "http://localhost:2007/RPC2";
    url[8]= "http://localhost:2008/RPC2";
    url[9]= "http://localhost:2009/RPC2";
    url[10]= "http://localhost:2010/RPC2";
    char * const methodName = "sample.add";



    xmlrpc_env env;
    xmlrpc_int adder;

    /*For synchronous*/
    xmlrpc_value * resultP;
    xmlrpc_int32 sum;
    const char * serverUrl = "";
    int sync_count = 0;


    /* Initialize our error environment variable */
    xmlrpc_env_init(&env);

    /* Create the Xmlrpc-c client object */
    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    die_if_fault_occurred(&env);
#if 1
  /**********Synchronous*************/

    /* Make the remote procedure call */

    int iter_num_of_requests;
    int i;

   for (iter_semantic = 0; iter_semantic < 3; iter_semantic++)
   { 

      for (iter_num_of_requests = 0; iter_num_of_requests <num_of_requests; iter_num_of_requests++)
      {
        sync_count = 0;
        sync_temp.temp_head_count = 0;
        sync_temp.temp_tail_count = 0;
        sync_temp.state = 1;
        average_time = 0;
        
        for (i=0; i<num_of_servers; i++)
        {
          serverUrl = url[i];
          srand(time(NULL));
          gettimeofday(&start, NULL);  
          resultP = xmlrpc_client_call(&env, serverUrl, methodName,
                                 "(ii)", (xmlrpc_int32) 5, (xmlrpc_int32) 7);
          die_if_fault_occurred(&env);

          /* Get our sum and print it out. */
          xmlrpc_read_int(&env, resultP, &sum);
          die_if_fault_occurred(&env);

          sync_count++;
          //printf("Sum = %d, i = %d, iter_semantic = %d\n", sum, i, iter_semantic); 
          if (sum == 1)
          sync_temp.temp_head_count++;
          else if (sum == 0)
          sync_temp.temp_tail_count++; 

          switch(iter_semantic)
          {
            case 0:
             // printf("In case 0\n");
              if(sync_count == num_of_servers) //count = num_of_servers
	      {	
                sync_count = 0;
                 //printf("Inner case 0\n");
 		if (sync_temp.temp_head_count == num_of_servers)
                statistic[0].head_count++;
                else if (sync_temp.temp_tail_count == num_of_servers)
                statistic[0].tail_count++;
                else
                statistic[0].rpc_fail_count++; 
          	gettimeofday(&end, NULL);
                  
          	average_time = average_time + ((end.tv_sec*1000 + end.tv_usec*10) - (start.tv_sec*1000 + start.tv_usec*10));
              }
              break;

            case 1:
              if (sync_count == 1)
              {

                if (sync_temp.temp_head_count > 0)
                statistic[1].head_count++;
                else if (sync_temp.temp_tail_count > 0)
                statistic[1].tail_count++; 
          	gettimeofday(&end, NULL);
          	average_time = average_time + ((end.tv_sec*1000 + end.tv_usec*10) - (start.tv_sec*1000 + start.tv_usec*10));

              }
              break;

            case 2:
              if (((sync_temp.temp_head_count > num_of_servers/2) || (sync_temp.temp_tail_count > num_of_servers/2)) && (sync_temp.state == 1))
              {
               // printf("Done with majority\n");
                if (sync_temp.temp_head_count > sync_temp.temp_tail_count)
                statistic[2].head_count++; 

                else if (sync_temp.temp_tail_count > sync_temp.temp_head_count)
                statistic[2].tail_count++;
		sync_temp.state = 0;
                 
          	gettimeofday(&end, NULL);
                average_time = average_time + ((end.tv_sec*1000 + end.tv_usec*10) - (start.tv_sec*1000 + start.tv_usec*10));
 
              }
              break;

            default:
             printf("Nothing found in sync\n");
          }
           
         /* Dispose of our result value. */
         xmlrpc_DECREF(resultP);


        }

      }
        average_time = average_time / num_of_requests;
        print_stats(iter_semantic, 1, average_time);
    }


        clear_all_stats();

#endif

#if 1
    /*****************For Asynchronous*************/
    for (iter_semantic = 0; iter_semantic < 3; iter_semantic++) {
        //printf("Making XMLRPC call to server url '%s' method '%s' and semantic '%d' \n", url, methodName, semantic.semantic_type);


	
        gettimeofday(&start, NULL);  
        /* request the remote procedure call */
        xmlrpc_client_call_asynch_temp(url, num_of_servers, adder, methodName,
                                  handle, NULL,
                                  "(ii)", (xmlrpc_int32) 5);
        gettimeofday(&end, NULL);  
        average_time = average_time + ((end.tv_sec*1000 + end.tv_usec/10) - (start.tv_sec*1000 + start.tv_usec/10));

	average_time = average_time / num_of_requests;
        print_stats(iter_semantic, 2, average_time);
        temp.semantic_type++; 
        temp.count = 0;
        temp.flag = 0;
        die_if_fault_occurred(&env);



    }
    
#endif

    //printf("All RPCs finished.\n");

    /* Destroy the Xmlrpc-c client object */
    xmlrpc_client_cleanup();

    return 0;
}
