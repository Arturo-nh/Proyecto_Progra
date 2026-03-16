#include <stdio.h>

int main() {

    char placas[7]; //23 bits
    char color; // 4 bits
    char verificacion; // 1 bit
    char combustible; // 2 bits
    int velocidad_mh; // 8 bits
    int velocidad_limite; // 7 bits
    int multa; // 8 bits
    
    scanf("%6s %c [%d|%d] %c %c", placas, &color, &velocidad_mh, &velocidad_limite, &verificacion, &combustible);

    float velocidad_km = (velocidad_mh+999)/1000;

}

/*usamos unsigned long long ya que sumando todos los bits necesarios que usa cada variable para guardar la info
suman 43 bits */

unsigned long long encode (char placas[7],char color,char verificacion,char combustible,int velocidad_km,int velocidad_limite){
    unsigned long long x = 0;
    unsigned long datos = 0;

    //queremos recorrer cada variable ciertas posiciones para meterlas a la variable x por lo que usamos <<

    datos |= (placas[0] - 'A');
    datos <<= 5; // 01001 00000
    datos |= (placas[1] - 'A');
    datos <<= 5; // 01001 01110 00000
    datos |= (placas[2] - 'A');
    datos <<= 5; // 01001 01110 10111 0000
    datos |= (placas[4] - '0');
    datos <<= 4; // 01001 01110 10111 1010 0000
    datos |= (placas[5] - '0');
    // 01001 01110 10111 1010 1110

    int color_code;
    if(color=='R') 
        color_code=0;
    else if(color=='A') 
        color_code=1;
    else if(color=='B') 
        color_code=2;
    else 
        color_code=3;
    
    
    int verif_code=(verificacion=='Y')?1:0;

    int combus_code;
    if(combustible=='G')
        combus_code==0;
    else if(combustible=='D')
        combus_code==1;
    else if(combustible=='E')
        combus_code==3;

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
    x << 1;
    x |= verif_code;

    return x;
}

void central (unsigned long long x){
    
    



    if (velocidad_km>velocidad_limite) {
        int km_excedidos = velocidad_km-velocidad_limite;
        int precio = 50;

        if (km_excedidos>=20)
            precio=100;
        else if (km_excedidos >= 40)
            precio = 200;
        else if (km_excedidos >= 60)
            precio = 400;
        else if (km_excedidos >= 80)
            precio = 800;
        multa = precio*km_excedidos;
    }


    if (velocidad_km > velocidad_limite && verificacion == 'N') {

        if (combustible == 'E')
            multa = multa * 1.25;
        else
            multa = multa * 1.50;
    }
}