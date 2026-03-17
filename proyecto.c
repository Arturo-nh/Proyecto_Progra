#include <stdio.h>

/*usamos unsigned long long ya que sumando todos los bits necesarios que usa cada variable para guardar la info
suman 43 bits */

unsigned long long encode(char placas[7],char color,char verificacion,char combustible,int velocidad_km,int velocidad_limite){
    unsigned long long x = 0;
    unsigned long long datos = 0;

    //queremos recorrer cada variable ciertas posiciones para meterlas a la variable x por lo que usamos 

    datos |= (placas[0] - 'A');
    datos <<= 5; // 01001 00000
    datos |= (placas[1] - 'A');
    datos <<= 5; // 01001 01110 00000
    datos |= (placas[2] - 'A');
    datos <<= 4; // 01001 01110 10111 0000
    datos |= (placas[4] - '0');
    datos <<= 4; // 01001 01110 10111 1010 0000
    datos |= (placas[5] - '0');
    // 01001 01110 10111 1010 1110

    //Aquí ya codificamos las opciones de color
    int color_code;
    if(color=='R') 
        color_code=0;
    else if(color=='A') 
        color_code=1;
    else if(color=='B') 
        color_code=2;
    else 
        color_code=3;
    
  /*Aquí usamos el operador ternario para asignar el código de si tiene o no la verificación; y está fácil
  en ternario porque solo son dos valores posibles  
  */
    int verif_code=(verificacion=='Y')?1:0;

//Nuestra codificación para los tipos de combustible
    int combus_code;
    if(combustible == 'G')
        combus_code = 0;
    else if(combustible == 'D')
        combus_code = 1;
    else if(combustible =='E')
        combus_code = 2;

    /* pues aqui ya es meter a la variable x, cada una de nuestras codificacions ya convertidas 
    con sus respectivos espacios en bits
    */

    x <<= 23; 
    x |= datos;
    x <<= 2;
    x |= color_code;  
    x <<= 8;
    x |= velocidad_km;
    x <<= 7;
    x |= velocidad_limite;
    x <<= 1;
    x |= verif_code;
    x <<= 2;
    x |= combus_code;

    return x;
}

void central(unsigned long long x){
int velocidad_km;
int velocidad_limite;
int verif_code;
int color_code;
int combus_code;
unsigned long long datos;
float multa_velocidad = 0;
float multa_no_refrendo = 0;
float total = 0;

    /* Lo que sigue ahora es descodificar nuestra variable e ir sacando cada dato que nos
    interesa 1 por 1; esto lo logramos a partir de separarlos por su valor de bits que definimos al inicio*/
combus_code = x & 3;
    x >>=2;
verif_code = x & 1; //aplicamos la máscara para sacar solo los valores que necesitamos (1 porque esta en la ultima posición y es solo 1 bit)
    x >>= 1; //y recorremos nuestra variable para eliminar la que ya sacamos
velocidad_limite = x & 127; //7 bits equivalen a 127 en decimal para que nos queden puros 1; 128 sería 10000000 y no es lo que necesitamos
    x >>= 7;
velocidad_km = x & 255;
    x >>= 8; 
color_code = x & 3;
    x>>=2;
datos = x & 8388607;
// ya aquí no hace falta hacer corrimiento porque terminamos de usar x

//Ahora si descodificamos todo por completo//

char placas[7];
placas[0] = (datos >> 18) + 'A'; 
placas[1] = ((datos >> 13) & 31) + 'A'; /*vamos de 5 en 5 recorriendo datos porque 
cada letra es de 5 bits; eñ 31 es la mascara que necesitamos para solo esos 5 bits */
placas[2] = ((datos >> 8) & 31) + 'A';
placas[3] = '-';
placas[4] = ((datos >> 4) & 15) + '0'; //aqui recorremos 4 porque un numero es de 4 bits. La mascara es de 15 porque a eso equivalen 4 bits prendidos.
placas[5] = (datos & 15) + '0';
placas[6] = '\0';

char color_nombre[10];
 if (color_code == 0){
     sprintf(color_nombre, "Rojo");
 }
 else if (color_code == 1){
     sprintf(color_nombre, "Azul");
 }
 else if (color_code == 2){
     sprintf(color_nombre, "Blanco");
     }
 else if (color_code == 3){
     sprintf(color_nombre, "Negro");
 }

char combus_nombre[10];

if (combus_code == 0){
    sprintf(combus_nombre, "Gasolina");
}
else if (combus_code == 1){
    sprintf(combus_nombre, "Diesel");
}
else if (combus_code == 2){
    sprintf(combus_nombre, "Electrico");
}

float km_excedidos = 0;
float precio = 0; 

if (velocidad_km>velocidad_limite) {
        km_excedidos = velocidad_km-velocidad_limite;
        precio = 50;
/*Aqui si checó nuestro historial de código, podrá ver que pusimos antes los if de menor a mayor, lo cual
        es un error ya que una vez que encuentre un TRUE se saltará todos los demás ifs. Es por eso
        que los pusimos de mayor a menor, para que si ponga la multa correcta*/
        if (km_excedidos >= 80)
            precio = 800;
        else if (km_excedidos >= 60)
            precio = 400;
        else if (km_excedidos >= 40)
            precio = 200;
        else if (km_excedidos >= 20)
            precio = 100;
        
        multa_velocidad = precio*km_excedidos;
    }

    if (verif_code == 0) {
        if (km_excedidos > 0){
            multa_no_refrendo = (combus_code == 2) ? multa_velocidad * 0.25 : multa_velocidad * 0.50;
        }
        else{
            multa_no_refrendo = 1500;
        }
    }

    total = multa_velocidad + multa_no_refrendo;

printf(" \n");
printf("Informacion Multa\n");
printf(" \n");
printf("Color: %s\n", color_nombre);
printf("Placas: %s\n", placas);
printf("----------------------------------------------------------------\n");
printf("Velocidad Actual : %d km / h\n", velocidad_km);
printf("Limite de velocidad en via : %d km / h\n", velocidad_limite);
printf("Refrendo : %s\n", verif_code ? "PAGADO" : "VENCIDO");
printf("\nDesgloce de multa :\n");
printf("----------------------------------------------------------------\n");
if (multa_velocidad > 0)
    printf("- Exceso de velocidad (%0.0f km x $%0.2f pesos)\n$ %8.2f MXN\n", km_excedidos, precio, multa_velocidad);
if (multa_no_refrendo > 0) 
    printf("- Sin Refrendo, %.2fx  $ +%8.2f MXN\n", (combus_code == 2 ? 1.25 : 1.50), multa_no_refrendo);
printf("----------------------------------------------------------------\n");
printf("Total: $ %8.2f MXN\n", total);
}

int main() {

    char placas[7]; //23 bits
    char color; // 4 bits
    char verificacion; // 1 bit
    char combustible; // 2 bits
    int velocidad_mh; // 8 bits
    int velocidad_limite; // 7 bits
    
    scanf("%6s %c [%d|%d] %c %c", placas, &color, &velocidad_mh, &velocidad_limite, &verificacion, &combustible);
    
    float velocidad_km = (velocidad_mh+999) / 1000.0;

    unsigned long long x = encode(placas, color, verificacion, combustible, (int)velocidad_km, velocidad_limite);

    central(x);
}