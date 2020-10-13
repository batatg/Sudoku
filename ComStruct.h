/*
 * ComsTruct.h
 *
 *  Created on: 25 במאי 2019
 *      Author: אוהד גזית
 */



#ifndef COMSTRUCT_H_
#define COMSTRUCT_H_

/*typedef struct changes changes;
typedef struct lnkd lnkd;
*/

#include "Commands.h"
#include "LinkedLst.h"
#include "EzerCom.h"


int com_manager(/*int ** mat,*/com_data * data,game_data * game);
void com_name(com_data * data,int status,char * token);
void set_param_check(com_data * data,game_data * game, char * array[]);
void param_check(/*int ** mat,*/com_data * data, game_data * game);

#endif /* COMSTRUCT_H_ */
