//////////////////////////////////////////////////
// Rutinas de impresion en pantalla y digitos Hardware
//////////////////////////////////////////////////
#define CANT_MOD_DIG 3
#define PIN_DATA_DIG D7 // Datos para generar el objeto LedControl de los digitos
#define PIN_CS_DIG D8
#define PIN_CLK_DIG D5
#define CANT_MOD_PANT 8 // Datos para generar el objeto MD_parola de la pantalla
#define PIN_DATA_PANT D7
#define PIN_CS_PANT D6
#define PIN_CLK_PANT D5
#define TIPO_PANT MD_MAX72XX::FC16_HW // Azules= FC16_HW - Verdes= GENERIC_HW

#define PIN_BUZZER D3 // pin de sonido
#define PIN_LED D4    // LED_BUILTIN

//////////////////////////////////////////////////
// Configuraciones animaciones pantalla
//////////////////////////////////////////////////
// #define EN_PRINT 1
#define EN_SCROLL 1
#define EN_SPRITE 1
#define EN_MISC 1
#define EN_WIPE 1
#define EN_SCAN 1
#define EN_OPNCLS 1
#define EN_SCR_DIA 1
#define EN_GROW 1

struct sCatalog
{
        textEffect_t effect; // text effect to display
        const char *psz;     // text string nul terminated
        uint16_t speed_min;  // speed multiplier of library min
        uint16_t speed_max;  // speed multiplier of library max
};
sCatalog catalog[] =
    {
#if EN_PRINT
        {PA_PRINT, "PRINT", 1, 1},
#endif
#if EN_SCROLL
        {PA_SCROLL_UP, "SC_U", 5, 100},
        {PA_SCROLL_DOWN, "SCR_D", 5, 100},
        {PA_SCROLL_LEFT, "SCR_L", 3, 60},
        {PA_SCROLL_RIGHT, "SCR_R", 3, 60},
#endif
#if EN_SPRITE
        {PA_SPRITE, "SPRIT", 8, 60},
#endif
#if EN_MISC
        {PA_SLICE, "SLICE", 1, 3},
        {PA_MESH, "MESH", 30, 100},
        {PA_FADE, "FADE", 30, 120},
        {PA_DISSOLVE, "DSLVE", 80, 150},
        {PA_BLINDS, "BLIND", 6, 150},
        {PA_RANDOM, "RAND", 1, 50},
#endif
#if EN_WIPE
        {PA_WIPE, "WIPE", 1, 40},
        {PA_WIPE_CURSOR, "WPE_C", 1, 40},
#endif
#if EN_SCAN
        {PA_SCAN_HORIZ, "SCNH", 8, 80},
        {PA_SCAN_HORIZX, "SCNHX", 8, 80},
        {PA_SCAN_VERT, "SCNV", 10, 100},
        {PA_SCAN_VERTX, "SCNVX", 10, 100},
#endif
#if EN_OPNCLS
        {PA_OPENING, "OPEN", 3, 70},
        {PA_OPENING_CURSOR, "OPN_C", 4, 80},
        {PA_CLOSING, "CLOSE", 3, 70},
        {PA_CLOSING_CURSOR, "CLS_C", 4, 80},
#endif
#if EN_SCR_DIA
        {PA_SCROLL_UP_LEFT, "SCR_UL", 7, 120},
        {PA_SCROLL_UP_RIGHT, "SCR_UR", 7, 120},
        {PA_SCROLL_DOWN_LEFT, "SCR_DL", 7, 120},
        {PA_SCROLL_DOWN_RIGHT, "SCR_DR", 7, 120},
#endif
#if EN_GROW
        {PA_GROW_UP, "GRW_U", 7, 120},
        {PA_GROW_DOWN, "GRW_D", 7, 120},
#endif
};

//////////////////////////////////////////////////
// Configuraciones de las teclas-botones
//////////////////////////////////////////////////

u_int8_t DIGITAL_SWITCH_PINS[] = {D1, D2}; // switches connected to these pins
u_int8_t DIGITAL_SWITCH_ACTIVE = LOW;      // digital signal when switch is pressed 'on'

#define VERIF_PULSA 50 // tiempo de verificacion de pulsaciones
u_int8_t tecla = 0;
u_int8_t accTecla = 0;
String nomAccTecla[] = {"NULL", "DOWN", "UP", "PRESS", "DOUBLE", "LONG", "REPEAT"};

//////////////////////////////////////////////////
// Configuraciones Varias
//////////////////////////////////////////////////
u_int16_t parpadeo_led = 1000; // Tiempo parpadeo led
#define REFRESH_DIG 150        // Refresco animacion Digitos

//////////////////////////////////////////////////
// Configuraciones WEB
//////////////////////////////////////////////////
boolean modo_offline = true;
const char *ssidWM = "Quin2022v5.0";
const char *passWM = "12345678";
const char *ssidAP = "Quin2022AP";
const char *passAP = "12345678";
const u_int8_t DNS_PORT = 53;
IPAddress ipAP = IPAddress(192, 168, 5, 1);
IPAddress gatewayAP = IPAddress(192, 168, 5, 1);
IPAddress subnetAP = IPAddress(255, 255, 255, 0);

const char *ssidSTA = "";
const char *passSTA = "";

String AGENCIA= "CASA";              // quien es el actuañizador: CASA, GANADORA, 5ESQUINAS, PILARES, COLORADA, CHAPULIN
IPAddress _ip = IPAddress(192, 168, 1, 33);
IPAddress _gw = IPAddress(192, 168, 1, 1);
IPAddress _sn = IPAddress(255, 255, 255, 0);
IPAddress dns1 = IPAddress(8, 8, 8, 8);
IPAddress dns2 = IPAddress(8, 8, 4, 4);

///////////////////////////////////////////////////////////////////
//// Configuraciones Firebase
///////////////////////////////////////////////////////////////////

#define API_KEY "AIzaSyDlWH2Cz0FyFXWtKby1kva6k8Us4bJAMBE"
#define USER_EMAIL "correodeprueba@prueba.com"
#define USER_PASSWORD "12345678"
#define DATABASE_URL "cartel-cabezas-2022-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "DATABASE_SECRET"
boolean f_grabar_firebase = false;     // Bandera grabar
boolean f_leer_actual_fire = true; // Bandera leer actualizacion
uint32_t actualizacion_FB = 0;          // actualizacion en BD
uint32_t actualizacion = 0;          // Actualizacion en hardware
String path = "/Numeros/";       // path de BD
#define VERIF_BD 150000           // tiempo verificacion actualizacion BD
uint8_t error_firebase=1;        //contador de errores en coneccion

///////////////////////////////////////////////////////////////////
//// Estructuras y datos
///////////////////////////////////////////////////////////////////

boolean f_sonido = true; // Activa sonido

u_int8_t jug_Actual = 0;         // Jugada mostrada
u_int8_t num_txt_Actual = 0;     // Texto mostrado, 0=hoy,etc, 1= primera,vespertina, 2= lunes, martes, etc.
boolean f_cambiar_jugada = true; // Si cambio la jugada actual

String n_jug_Actual; // Nombre jugada mostrada

String n_Jugada[] = {"La Previa", "La Primera", "Matutina", "Vespertina", "Nocturna"};           // Textos jugadas(Prim, Matu, etc.)
String n_Loteria[] = {"Provincia", "Ciudad", "Córdoba", "Santa Fé", "Entre Ríos", "Montevideo"}; // Textos loterias
u_int8_t pag_Jugada = 0;                                                                         // jugada mostrada en la pagina web
String n_pag_Jugada = n_Jugada[pag_Jugada];                                                      // Nombre jugada mostrada en la pagina web

struct n_salido // Estructura para guardar los numeros en memoria
{
        u_int16_t numero;  // Numero salido
        boolean f_mostrar; // si mostrarlo o no
};

u_int16_t eeDir = 0; // Indice para la memoria

u_int8_t brillo_textos = 10;     // Brillo de los textos
u_int16_t tiempo_jugada = 10000; // Tiempo para cambio de jugada
u_int16_t vel_texto = 10;        // Velocidad pasa texto

boolean f_anima_pant = false;  // Si animar texto o no
boolean f_anima_dig = false;   // Si animar digitos o no
boolean f_cambia_Dig = false;  // Si tengo que cambiar los digitos
boolean f_fin_Anim_Dig = true; // Si termino la animación de los digitos

/////////////////////////////////////////////////////
// datos para cargar los sueños
/////////////////////////////////////////////////////
struct s_sueños
{
        u_int8_t numero;
        String significado;
};

s_sueños mi_sueños[] = {
    {0, "Huevos"}, {1, "AGUA"}, {2, "Niño"}, {3, "S.Cono"}, {4, "La Cama"}, {5, "GATO"}, {6, "Perro"}, {7, "Revolver"}, {8, "Incendio"}, {9, "Arroyo"}, {10, "Leche"}, {11, "Minero"}, {12, "Soldado"}, {13, "YETA"}, {14, "Borracho"}, {15, "N.Bonita"}, {16, "Anillo"}, {17, "Desgracia"}, {18, "Sangre"}, {19, "Pescado"}, {20, "Fiesta"}, {21, "Mujer"}, {22, "LOCO"}, {23, "Cocinero"}, {24, "Caballo"}, {25, "Gallina"}, {26, "Misa"}, {27, "Peine"}, {28, "Cerro"}, {29, "S.Pedro"}, {30, "S.Rosa"}, {31, "LUZ"}, {32, "Dinero"}, {33, "Cristo"}, {34, "Cabeza"}, {35, "Pajarito"}, {36, "Manteca"}, {37, "Dentista"}, {38, "Piedras"}, {39, "Lluvia"}, {40, "El Cura"}, {41, "Cuchillo"}, {42, "Zapatillas"}, {43, "Balcón"}, {44, "Cárcel"}, {45, "VINO"}, {46, "Tomates"}, {47, "Muerto"}, {48, "M.q.Habla"}, {49, "Carne"}, {50, "El Pan"}, {51, "Serrucho"}, {52, "Madre"}, {53, "Barco"}, {54, "La Vaca"}, {55, "Música"}, {56, "La Caída"}, {57, "Jorobado"}, {58, "Ahogado"}, {59, "Plantas"}, {60, "La Virgen"}, {61, "Escopeta"}, {62, "Inundación"}, {63, "Casamiento"}, {64, "Llanto"}, {65, "Cazador"}, {66, "Lombriz"}, {67, "Mordida"}, {68, "Sobrinos"}, {69, "Vicios"}, {70, "Mue.Sueño"}, {71, "Excremento"}, {72, "Sorpresa"}, {73, "Hospital"}, {74, "G.Negra"}, {75, "Besos"}, {76, "Llamas"}, {77, "Pierna M."}, {78, "Ramera"}, {79, "Ladrón"}, {80, "La Bocha"}, {81, "Flores"}, {82, "Pelea"}, {83, "Mal Tiempo"}, {84, "Iglesia"}, {85, "Linterna"}, {86, "HUMO"}, {87, "Piojos"}, {88, "El Papa"}, {89, "RATA"}, {90, "Miedo"}, {91, "Excusado"}, {92, "Médico"}, {93, "Enamorado"}, {94, "Cementerio"}, {95, "Anteojos"}, {96, "Marido"}, {97, "Mesa"}, {98, "Lavandera"}, {99, "Hermano"}};

/////////////////////////////////////////////////////
// datos para cargar en la eprom 1era vez
/////////////////////////////////////////////////////

boolean f_most_Jug[] = {1, 1, 1, 1, 1};    // Mostrar o no una jugada(Previa, Prim, Matu, Vesp, Noctu)
boolean f_most_Jug_FB[] = {1, 1, 1, 1, 1}; // temporal para firebase
// numeros salidos y si mostrar o no
n_salido mi_n_salido[] = {
    {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 0}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 0}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 1}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 0}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 1}};
// numeros salidos y si mostrar o no para firebase
n_salido mi_n_salido_FB[] = {
    {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 0}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 0}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 1}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 0}, {1111, 1}, {2222, 1}, {3333, 1}, {4444, 1}, {5555, 1}, {6666, 1}};
