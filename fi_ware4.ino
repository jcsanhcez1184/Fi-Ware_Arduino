#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xBA }; //mac address de nuestro dispositivo

char url[]= "orion.lab.fi-ware.org"; //La dirección de conexion. Para este caso Orión de Fi-Ware
char ID[]= "/ngsi10/contextEntities/1000000/attributes/temperature"; //Ruta de alojamiento en broker
char token[]="lBiR8Pztrng2TMJdSlALgNt8KtHSrO3AFiHmPs3V5UYksZLbMbRher6S4qViBtKpzSe4r21Vo49lGBi4SoO4WQ"; //Token de autenticación
char tipo[]="PUT "; //Método HTTP, para el caso de escritura de datos de acuerdo con documentación es PUT
char payload[] = "{ \"value\" : \"3.45\" }"; //Aqui se deposita la carga util de información hacia el broker, en este caso se considera formato JSON
int port = 1026; //Puero de comunicación. Para el caso de orion es el 1026

char buffout[1500]; //Buffer de almacenamiento de salida hacia el ethernet shield de arduinp

//Esta función permite generar el formato HTTP para comunicación con Broker
void esc_buff(char*buffer,char * tipo,char* id,char* url,char*token,char* req) 
{
  //Variables y tabla estática de expresiones HTML invariantes para este caso. También se puede hacer dinámica o ser extraida de archivo externo
  int a;
  char *p;
  char *m;
  char lon[7];
  static char e1[]=" HTTP/1.1\r\n";
  static char e2[]="User-Agent: Arduino\r\n";
  static char e3[]="Host:";
  static char e4[]=":1026\r\n";
  static char e5[]="Content-Type: application/json\r\n";
  static char e6[]="Accept: application/json\r\n";
  static char e7[]="X-Auth-Token:";
  static char crlf[]="\r\n";
  static char e9[]="Content-Length:";
  
  
  
  a=0;
  p=req;
  while(*p++!=0)
   a++;
  sprintf(lon,"%i",a);
  
  p=buffer; 
  char *s[]={tipo,id,e1,e2,e3,url,e4,e5,e6,e7,token,crlf,e9,lon,crlf,crlf,req}; //Secuencia de colocación de datos en la cadena de salida
  for (a=0;a<17;a++)
  {
    m=s[a];
    while(*m!=0)
     *p++=*m++;
  }
  *p=0;
}

EthernetClient cliente;

void setup()
{
  Serial.begin(9600);
  if(!Ethernet.begin(mac)) //Conexión por DHCP. Suele tardar de 10 a 30 segundos en responder 
  {
    Serial.println("No se pudo iniciar DHCP");
    return;
  }
  Serial.println("Direccion IP:");
  Serial.println(Ethernet.localIP());
  delay(10);
  
}

void loop()
{
  float h=2.3;
  char num[10];
  char *l;
  char payloadl[30]="{ \"value\" : \"";
  char *k;
  
  h= 50*random(50)/50; //Se genera una variable aleatoria
  
  k=payloadl+13;
  l=num;
  dtostrf(h,5,2,num);
  while(*l!=0)
   *k++=*l++;
  *k++='"';
  *k++=' ';
  *k++='}';
  *k=0; //con todo lo anterior se le da formato JSON de la forma {"value" : "h"}
  
  if (!cliente.connect(url,port)) //Se intenta establecer la conexión
  {
   Serial.println("NO CONECTADO");
   return;
  }
  else
  {
   Serial.println("CONECTADO"); //Si esta conectado 
   esc_buff(buffout,tipo,ID,url,token,payloadl); //Se genera la cadena
   cliente.print(buffout);//Se envía a través de la red
      
  }
   delay(10);
  while(!cliente.available())
  {}
  while(cliente.available())
   cliente.read(); //Con lo anterior se tiran los datos de respuesta. Cabe aclarar que esto se hizo por falta de tiempo, ya que con estos datos se puede asegurar que las transacciones son exitosas.
  cliente.stop();
  Serial.print("Escrito: "); // Se muestra lo que se ha enviado
  Serial.println(h,DEC);
  delay(5000); //Se espera 5 segundos para re-envio de datos
  
  
}
