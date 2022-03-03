#define BYTESxTRAMA 34

struct __attribute__((__packed__)) Timestamp 
{
	uint8_t mes;
	uint8_t dia;
	uint16_t anio;
	uint8_t hora;
	uint8_t minuto;
	uint8_t sec;	
};

struct __attribute__((__packed__)) Transactions
{
	Timestamp *fecha_transaccion;
	char *vehicle_registration;
	char *product;
	int32_t mililiters;
	uint16_t transaction_id;
};

/**************************************************************************//**
* @brief 		Compara las fechas recibidas de la estructura de datos
* @param[in] 	Timestamp estructura de datos de fechas de trabsacciones
* @param[out]   Orden entre fechas
* @return		booleano
* @note			
******************************************************************************/
bool comparador(const Timestamp &d1, const Timestamp &d2)
{
    if (d1.anio < d2.anio)
        return true;
	
    if (d1.anio == d2.anio && d1.mes < d2.mes)
        return true;
	
    if (dd1.anio == d2.anio && d1.mes == d2.mes &&
                              d1.dia < d2.dia)
        return true;
		
	if (dd1.anio == d2.anio && d1.mes == d2.mes &&
                              d1.dia == d2.dia && d1.hora < d2.hora)
        return true;
		
	if (dd1.anio == d2.anio && d1.mes == d2.mes &&
                              d1.dia == d2.dia && d1.hora == d2.hora &&
							  d1.minuto < d2.minuto)
        return true;
		
	if (dd1.anio == d2.anio && d1.mes == d2.mes &&
                              d1.dia == d2.dia && d1.hora == d2.hora &&
							  d1.minuto == d2.minuto && d1.sec < d2.sec)
        return true;
     
    return false;
}

/**************************************************************************//**
* @brief 		Carga las transacciones recibidas en buffer en packed structure,
				ordena las transacciones por fechas y luego imprime el LOG
* @param[in] 	*data puntero a buffer de datos
				*log puntero a buffer de LOG
				transaction_count cantidad de transacciones procesadas
* @param[out]   Orden por fecha de las transacciones
* @return		Cantidad de bytes procesados
* @note			Se considera que el buffer de datos contiene en su direccion de 
				memoria inicial al menos una transaccion completa y que los datos 
				estan en forma contigua.
******************************************************************************/
int log_transactions(const char *data, char *log, size_t transaction_count)
{
	Transactions transaccion[];
	uint8_t count = 0;
	uint8_t cant_bytes=0;
	
	while( count < transaction_count)
	{
		// Se carga estructura de datos con los elementos del buffer. Se castea a int por leerse en ASCII
		
		transaccion[count].fecha_transaccion.mes = ((data[ 0 + cant_bytes ] - '0') * 10 + (data[ 1 + cant_bytes ] - '0'));
		transaccion[count].fecha_transaccion.dia = ((data[ 3 + cant_bytes ] - '0') * 10 + (data[ 4 + cant_bytes ] - '0'));
		transaccion[count].fecha_transaccion.anio = ((data[ 6 + cant_bytes ] - '0') * 1000 + (data[ 7 + cant_bytes ] * 100 - '0') + 
													  (data[ 8 + cant_bytes ] - '0') * 10 + (data[ 9 + cant_bytes ] - '0'));
		transaccion[count].fecha_transaccion.hora = ((data[ 11 + cant_bytes ] - '0') * 10 + (data[ 12 + cant_bytes ] - '0'));
		transaccion[count].fecha_transaccion.minuto = ((data[ 14 + cant_bytes ] - '0') * 10 + (data[ 15 + cant_bytes ] - '0'));
		transaccion[count].fecha_transaccion.sec = ((data[ 17 + cant_bytes ] - '0') * 10 + (data[ 18 + cant_bytes ] - '0'));
		
		transaccion[count].vehicle_registration[7] = data[ 19 + cant_bytes ];
		transaccion[count].vehicle_registration[6] = data[ 20 + cant_bytes ];
		transaccion[count].vehicle_registration[5] = data[ 21 + cant_bytes ];
		transaccion[count].vehicle_registration[4] = data[ 22 + cant_bytes ];
		transaccion[count].vehicle_registration[3] = data[ 23 + cant_bytes ];
		transaccion[count].vehicle_registration[2] = data[ 24 + cant_bytes ];
		transaccion[count].vehicle_registration[1] = data[ 25 + cant_bytes ];
		transaccion[count].vehicle_registration[0] = data[ 26 + cant_bytes ];
		
		transaccion[count].product = data[ 27 + cant_bytes ];
		
		transaccion[count].mililiters = ((data[ 28 + cant_bytes ] - '0') * 1000 + (data[ 29 + cant_bytes ] - '0') * 100 +
																(data[ 30 + cant_bytes ] - '0') * 10 + (data[ 31 + cant_bytes ] - '0'));
																
		transaccion[count].transaction_id = (data[ 32 + cant_bytes ] - '0') * 10 + (data[ 33 + cant_bytes ] - '0');
		
		// Avanzo a siguiente bloque de memoria 
		cant_bytes += BYTESxTRAMA;
		count++;		
		
	}
	
	// Se realiza el orden de datos a través de la función propia qsort();
	qsort((void*)transaccion, size, sizeof(uint32_t), comparador);
	
	registrarLog(log, transaction_count, &transaccion);
	
	imprimirLog (log, transaction_count);
	
	return cant_bytes;
	
}

/**************************************************************************//**
* @brief 		Se carga el buffer de registro LOG
* @param[in] 	*log: puntero a buffer de LOG
				transaction_count: cantidad de transacciones procesadas
				sortedTransactions: estructura de transacciones ya ordenadas 
* @param[out]   Registro del LOG
* @return		NONE
* @note			
******************************************************************************/

void registrarLog (char *log , size_t transaction_count, Transactions sortedTransactions)
{
	uint8_t count = 0;
	uint8_t cant_bytes=0;
	
	while( count < transaction_count)
	{
		log[0 + cant_bytes] = sortedTransactions[count].fecha_transaccion.mes >> 8;
		log[1 + cant_bytes] = sortedTransactions[count].fecha_transaccion.mes;
		log[2 + cant_bytes] = '/';
		log[3 + cant_bytes] = sortedTransactions[count].fecha_transaccion.dia >> 8;
		log[4 + cant_bytes] = sortedTransactions[count].fecha_transaccion.dia;
		log[5 + cant_bytes] = '/';
		log[6 + cant_bytes] = sortedTransactions[count].fecha_transaccion.anio >> 8;
		log[7 + cant_bytes] = sortedTransactions[count].fecha_transaccion.anio >> 16;
		log[8 + cant_bytes] = sortedTransactions[count].fecha_transaccion.anio >> 32;
		log[9 + cant_bytes] = sortedTransactions[count].fecha_transaccion.anio;
		log[10 + cant_bytes] = ' ';
		log[11 + cant_bytes] = sortedTransactions[count].fecha_transaccion.hora >> 8;
		log[12 + cant_bytes] = sortedTransactions[count].fecha_transaccion.hora;
		log[13 + cant_bytes] = ':';
		log[14 + cant_bytes] = sortedTransactions[count].fecha_transaccion.minuto >> 8;
		log[15 + cant_bytes] = sortedTransactions[count].fecha_transaccion.minuto;
		log[16 + cant_bytes] = ':';
		log[17 + cant_bytes] = sortedTransactions[count].fecha_transaccion.sec >> 8;
		log[18 + cant_bytes] = sortedTransactions[count].fecha_transaccion.sec;
		log[19 + cant_bytes] = sortedTransactions[count].vehicle_registration >> 256;
		log[20 + cant_bytes] = sortedTransactions[count].vehicle_registration >> 128;
		log[21 + cant_bytes] = sortedTransactions[count].vehicle_registration >> 64;
		log[22 + cant_bytes] = ' ';
		log[23 + cant_bytes] = sortedTransactions[count].vehicle_registration >> 32;
		log[24 + cant_bytes] = sortedTransactions[count].vehicle_registration >> 16;
		log[25 + cant_bytes] = sortedTransactions[count].vehicle_registration >> 8;
		log[26 + cant_bytes] = sortedTransactions[count].vehicle_registration;
		log[27 + cant_bytes] = sortedTransactions[count].product;
		log[28 + cant_bytes] = sortedTransactions[count].mililiters >> 32;
		log[29 + cant_bytes] = sortedTransactions[count].mililiters >> 16;
		log[30 + cant_bytes] = sortedTransactions[count].mililiters >> 8;
		log[31 + cant_bytes] = sortedTransactions[count].mililiters;
		log[32 + cant_bytes] = sortedTransactions[count].transaction_id >> 8;
		log[33 + cant_bytes] = sortedTransactions[count].transaction_id;
		
		count++;	
		cant_bytes += BYTESxTRAMA;
	}
}

/**************************************************************************//**
* @brief 		Se imprime en pantalla el buffer de registro LOG
* @param[in] 	*log: puntero a buffer de LOG
				transaction_count: cantidad de transacciones procesadas
				
* @param[out]   Registro del LOG
* @return		NONE
* @note			
******************************************************************************/
void imprimirLog(char *log, size_t transaction_count)
{
	uint8_t cant_bytes=0;
	
	for( uint8_t j=0; j < transaction_count; j++)
	{
		printf ("[");
		for( uint8_t i=0; i < 19; i++)
		{
			printf(%c, log[i + cant_bytes]);
		}
		printf ("]\t");
		printf ("id:\t");
		for( uint8_t i=0; i < 2; i++)
		{
			printf(%c, log[i + cant_bytes + 32]);
		}
		printf (",\t");
		printf ("reg:\t");
		for( uint8_t i=0; i < 8; i++)
		{
			printf(%c, log[i + cant_bytes + 19]);
		}
		printf (",\t");
		printf ("prod:\t");
		printf(%c, log[27]);
		printf (",\t");
		printf ("ltrs:\t");
		for( uint8_t i=0; i < 4; i++)
		{
			printf(%c, log[i +  cant_bytes + 28]);
		}
		printf ("/n");
		printf ("\n");
		
		cant_bytes += BYTESxTRAMA;
    }
}
