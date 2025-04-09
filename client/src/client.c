#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	

	// Usando el logger creado previamente

	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola! Soy un log");
	

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	clave = config_get_string_value(config, "CLAVE");


	// Loggeamos el valor de config
	log_info(logger,"Lei la IP --> %s",ip);
	log_info(logger,"Lei el PUERTO --> %s",puerto);
	log_info(logger,"Lei la CLAVE --> %s",clave);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger,"El puerto de conexion es: %i",conexion);
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje("Hola soy un mensaje de prueba",conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	// Liberamos todo al terminar el programa:
	terminar_programa(conexion, logger, config);


	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	printf("\n El Cliente Termino...");
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log","TP0",1,LOG_LEVEL_INFO);
	if(nuevo_logger  == NULL){
		printf("Hubo un error al crear el Logger \n");
		//abort();
		exit(1);
		}
		
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("./cliente.config") ;
	if(nuevo_config  == NULL){
		printf("Hubo un error al crear el Archivo de Config");
		//abort();
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	//  La primera te la dejo de yapa
	// leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(1){
		leido = readline("> ");

		if (leido) {
            add_history(leido);
			log_info(logger,"Lei por pantalla: %s", leido);
        }
        //if (!strncmp(leido, "", 4)) {
		if (!strcmp(leido, "")) {
            free(leido);
            break;
        }

        free(leido);
	}
	return(0);
	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido_para_paquete;
	t_paquete* paquete = crear_paquete();


	leido_para_paquete = readline("> ");
	while(strcmp(leido_para_paquete, "") != 0){
			agregar_a_paquete(paquete,leido_para_paquete, strlen(leido_para_paquete) + 1 );
			free(leido_para_paquete);
			leido_para_paquete = readline("> ");
	}
	// while(1){
	// 	leido_para_paquete = readline("> ");

	// 	if (leido_para_paquete) {
    //         agregar_a_paquete(paquete,leido_para_paquete, strlen(leido_para_paquete) + 1 );
    //     }
	// 	if (!strcmp(leido_para_paquete, "")) {
    //         free(leido_para_paquete);
    //         break;
    //     }

	//} 
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	   enviar_paquete(paquete,conexion);
	   eliminar_paquete(paquete);
}

// LIBERO LOS ESPACIOS DE MEMORIA:
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger != NULL){log_destroy(logger);}
	if(config != NULL){config_destroy(config);}
	liberar_conexion(conexion);
}
