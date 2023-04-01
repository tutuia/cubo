#include <Servo.h>
#include <Wire.h>
#include <ArduinoNvs.h>
#include <Arduino.h>

#define S1 32
#define S2 33
#define S3 25

char faces_color[7];
char middle_colors[25];
char corner_colors[25];  

//char faces_color[] = "yobrgw";
//char middle_colors[] = "ryyyoooybbbbrwrrggggwwow";
//char corner_colors[]="rryyyooybbbbrwwrggggwwoo";  

// servos para manipulação do cubo
Servo servo_F;
Servo servo_R;
Servo servo_B;
Servo servo_L;

//servos de posição
Servo servoPos_F;
Servo servoPos_R;
Servo servoPos_B;
Servo servoPos_L;

const int posZero = 0;
const int base_90_FB = 41;
const int base_90_L = 42;
const int base_90_R = 30;
int claw_F90 = 80;
int claw_F180 = 170;
int claw_B90 = 80;
int claw_B180 = 170;
int claw_L90 = 80;
int claw_L180 = 172;
int claw_R90 = 70;  
int claw_R180 = 180;  

void setup() {

  memset(faces_color, 0, sizeof(faces_color));
  memset(middle_colors, 0, sizeof(middle_colors));
  memset(corner_colors, 0, sizeof(corner_colors));

  ArduinoNvs mynvs;

  servoPos_F.attach(19);
  servoPos_R.attach(18);
  servoPos_B.attach(5);
  servoPos_L.attach(17);

  servo_F.attach(16);
  servo_R.attach(4);
  servo_B.attach(2);
  servo_L.attach(15);

  mynvs.begin("customNs");

  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  delay(200);
  Serial.begin(115200);
  Serial.print("\r\nstarting....");

  Wire.begin(21, 22);  
  Wire1.begin(14, 27);

// Teste de movimento das garras
//#define TEST
#ifdef TEST 
  while(1){
    if(Serial.available() > 0){
      char c1;
        c = Serial.read();
        Serial.write(c);

      switch(c1){
        case '0':
          F_clockwise();
        break;

        case '1':
          L_clockwise();
        break;

        case '2':
          R_clockwise();
        break;

        case '3':
          B_clockwise();
        break;
      }
    }
  }      

#endif

  while (!Serial);             
  Serial.println("\nI2C Scanner");

  for (int i = 0; i < 8; i++){
    mux(i);
    i2c_scanner(0);
  }

  i2c_scanner(1);

  claw_F90 = mynvs.getInt("F90");
  Serial.printf("F90: %i\n", claw_F90);

  claw_R90 = mynvs.getInt("R90");
  Serial.printf("R90: %i\n", claw_R90);

  claw_B90 = mynvs.getInt("B90");
  Serial.printf("B90: %i\n", claw_B90);

  claw_L90 = mynvs.getInt("L90");
  Serial.printf("L90: %i\n", claw_L90);
// 180

  claw_F180 = mynvs.getInt("F180");
  Serial.printf("F180: %i\n", claw_F180);

  claw_R180 = mynvs.getInt("R180");
  Serial.printf("R180: %i\n", claw_R180);

  claw_B180 = mynvs.getInt("B180");
  Serial.printf("B180: %i\n", claw_B180);

  claw_L180 = mynvs.getInt("L180");
  Serial.printf("L180: %i\n", claw_L180);

  Serial.print("\r\nwaiting... deseja calibrar servos? (S) ");

  char calibre[1];
  Serial.setTimeout(10000);
  Serial.readBytesUntil('\n', calibre, 1);

  if(calibre[0] == 'S'){
    Serial.print("calibrando...");

    int x = 0;
    char n[10];

    while(x != -1){
        Serial.print("<F> entre com um valor próximo a 90\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("F90", x);
          servo_F.write(x);
          claw_F90 = x;
        }
    }
    servo_F.write(0);
    x = 0;
    while(x != -1){
        Serial.print("<R> entre com um valor próximo a 90\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("R90", x);
          servo_R.write(x);
          claw_R90 = x;
        }
    }
    servo_R.write(0);
    x = 0;
    while(x != -1){
        Serial.print("<B> entre com um valor próximo a 90\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("B90", x);
          servo_B.write(x);
          claw_B90 = x;
        }
    } 
    servo_B.write(0);
    x = 0;
    while(x != -1){
        Serial.print("<L> entre com um valor próximo a 90\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("L90", x);
          servo_L.write(x);
          claw_L90 = x;
        }
    } 
    servo_L.write(0);
    // Verificando ângulos de 180 graus
    x = 0;

        while(x != -1){
        Serial.print("<F> entre com um valor próximo a 180\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("F180", x);
          servo_F.write(x);
          claw_F180 = x;
        }
    }
    servo_F.write(0);
    x = 0;
    while(x != -1){
        Serial.print("<R> entre com um valor próximo a 180\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("R180", x);
          servo_R.write(x);
          claw_R180 = x;
        }
    }
    servo_R.write(0);
    x = 0;
    while(x != -1){
        Serial.print("<B> entre com um valor próximo a 180\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("B180", x);
          servo_B.write(x);
          claw_B180 = x;
        } 
    } 
    servo_B.write(0);
    x = 0;
    while(x != -1){
        Serial.print("<L> entre com um valor próximo a 180\n");
        Serial.readBytesUntil('\n', n, 9);
        x = atoi(n);
        Serial.printf("valor digitado: %i\n", x);
        if(x != -1){
          mynvs.setInt("L180", x);
          servo_L.write(x);
          claw_L180 = x;
        }   
    } 
    servo_L.write(0); 
}

  servo_F.write(0);
  servo_L.write(0);
  servo_B.write(0);
  servo_R.write(0);

  recebe_cubo();
 
  Serial.write("\n ##############################################################################");
  Serial.write("\n Informe as iniciais dos nomes em inglês das cores: ");
  Serial.write("\n exemplo: informe w se a cor for branca");  
  Serial.write("\n na seguinte ordem 012345678 percorrendo a face em sentido horário onde:");
  Serial.write("\n 0 | 1 | 2");
  Serial.write("\n ----------");
  Serial.write("\n 7 | 8 | 3");
  Serial.write("\n ----------");  
  Serial.write("\n 6 | 5 | 4");

  Serial.write("\n\n EXEMPLO: se");
  Serial.write("\n b | y | o");
  Serial.write("\n ----------");
  Serial.write("\n r | r | y");
  Serial.write("\n ----------");  
  Serial.write("\n o | w | g");
  Serial.write("\n\n INFORMAR: byoygworr");
  Serial.write("\n ##############################################################################");
 
  char c;
  int i = 0;
  while(i<6){
    int j = 0;
    int aux_mid = 0;
    int aux_corner = 0;
    Serial.printf("\n\n Face %i\n",i+1);

    while(j<9){
      if(Serial.available() > 0){
        c = Serial.read();
        Serial.write(c);

        if(c != '\n'){
          if(j==8){
            faces_color[i] = c;
            j++;
            }else if(j%2 == 0){
              corner_colors[(4*i)+aux_corner] = c;
              aux_corner++;
              j++;
              }else{
                middle_colors[(4*i)+aux_mid] = c;
                aux_mid++;
                j++;
                }
          }  
        }
      }
      
    i++;

  Serial.write("\n\n Cores das quinas:");
  Serial.printf(corner_colors);

  Serial.write("\n\n Cores do meio:");
  Serial.printf(middle_colors);

  Serial.write("\n\n Cores das faces:");
  Serial.printf(faces_color);

    if(i == 1){
      turn_front_2up();
      turn_clockwise();
      }else if(i == 5){
        turn_counterClock();
        turn_counterClock();
        turn_front_2up();
        }else if(i == 6){
          turn_up_2front();
          turn_up_2front();
          }else turn_counterClock();      
    }


  Serial.write("\n\n Cores das quinas:");
  Serial.printf(corner_colors);
  Serial.write("\n\n Cores do meio:");
  Serial.printf(middle_colors);
  Serial.write("\n\n Cores das faces:");
  Serial.printf(faces_color);

  Serial.println(sizeof(middle_colors));
  Serial.println(sizeof(corner_colors));

  Serial.write("\n Iniciando solucao.....");
  delay(2000);
  map_middle();
  solta_cubo();
  return;
  
  map_corners();
  solta_cubo();

  Serial.write("\n\n Cores das quinas:");
  Serial.printf(corner_colors);

  Serial.write("\n\n Cores do meio:");
  Serial.printf(middle_colors);

  Serial.write("\n\n Cores das faces:");
  Serial.printf(faces_color);
}

void mux(int ch){

  int s1, s2, s3;
  s1 = ch & 1;
  s2 = (ch & 2) >> 1; 
  s3 = (ch & 4) >> 2;

  digitalWrite(S1, s1);
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
}


///////////////////////////////////////////////////// Setups  //////////////////////////////////////////////////////////////////////////
void i2c_scanner(int ch){
  {
  byte error, address;
  int nDevices;
  char s[100];
  sprintf(s, "Scanning... %d", ch);
  Serial.println(s);
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    if (ch == 0){
Wire.beginTransmission(address);
    error = Wire.endTransmission();
    }
    else{
      Wire1.beginTransmission(address);
    error = Wire1.endTransmission();
    }
    
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
         Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      nDevices++;
     }
     else if (error==4)
     {
      Serial.print("Unknown error at address 0x");
      if (address<16)
         Serial.print("0");
      Serial.println(address,HEX);
     }    
    }
    if (nDevices == 0)
       Serial.println("No I2C devices found\n");
    else
       Serial.println("done\n");
    delay(500);           // wait 5 seconds for next scan
}
}

void putting_middle_in_place(int whereTo) {
  Serial.print("\n Ir para: ");
  Serial.print(whereTo);
  if (whereTo < 8) {
    if (whereTo == 0) {
      Lw_clockwise();
      D_counterClock(1);
      S_counterClock(2);
      swap();
      S_counterClock(2);
      D_clockwise();
      Lw_counterClock(1);

    }
    else if (whereTo == 1) {
      swap();
    }
    else if (whereTo == 2) {
      // BUFFER
    }
    else if (whereTo == 3) {
      L_counterClock(2);
      S_counterClock(2);
      swap();
      S_counterClock(2);
      L_counterClock(2);
    }
    else if (whereTo == 4) {
      S_clockwise();
      swap();
      S_counterClock(1);

    }
    else if (whereTo == 5) {
      L_counterClock(1);
      S_clockwise();
      swap();
      S_counterClock(1);
      L_clockwise();

    }
    else if (whereTo == 6) {
      L_counterClock(2);
      S_clockwise();
      swap();
      S_counterClock(1);
      L_counterClock(2);

    }
    else if (whereTo == 7) {
      L_clockwise();
      S_clockwise();
      swap();
      S_counterClock(1);
      L_counterClock(1);
    }
  }
  else if (whereTo > 15) {
    if (whereTo == 16) {
      Lw_clockwise();
      D_clockwise();
      S_counterClock(1);
      swap();
      S_clockwise();
      D_counterClock(1);
      Lw_counterClock(1);

    }
    else if (whereTo == 17) {
      L_counterClock(1);
      S_counterClock(2);
      swap();
      S_counterClock(2);
      L_clockwise();

    }
    else if (whereTo == 18) {
      D_counterClock(1);
      S_counterClock(1);
      swap();
      S_clockwise();
      D_clockwise();

    }
    else if (whereTo == 19) {
      E_clockwise(1);
      L_clockwise();
      S_clockwise();
      swap();
      S_counterClock(1);
      L_counterClock(1);
      E_counterClock(1);

    }
    else if (whereTo == 20) {
      D_counterClock(1);
      S_counterClock(2);
      swap();
      S_counterClock(2);
      D_clockwise();

    }
    else if (whereTo == 21) {
      D_counterClock(2);
      S_counterClock(2);
      swap();
      S_counterClock(2);
      D_counterClock(2);

    }
    else if (whereTo == 22) {
      D_clockwise();
      S_counterClock(2);
      swap();
      S_counterClock(2);
      D_counterClock(1);

    }
    else if (whereTo == 23) {
      S_counterClock(2);
      swap();
      S_counterClock(2);
    }
  }
  else {
    if (whereTo == 8) {
      //BUFFER
    }
    else if (whereTo == 9) {
      E_counterClock(1);
      L_counterClock(1);
      S_clockwise();
      swap();
      S_counterClock(1);
      L_clockwise();
      E_clockwise(1);
    }
    else if (whereTo == 10) {
      D_clockwise();
      S_counterClock(1);
      swap();
      S_clockwise();
      D_counterClock(1);
    }
    else if (whereTo == 11) {
      L_clockwise();
      S_counterClock(2);
      swap();
      S_counterClock(2);
      L_counterClock(1);

    }
    else if (whereTo == 12) {
      S_clockwise();
      D_counterClock(2);
      S_counterClock(2);
      swap();
      S_counterClock(2);
      D_counterClock(2);
      S_counterClock(1);
    }
    else if (whereTo == 13) {
      E_clockwise(2);
      L_counterClock(1);
      S_clockwise();
      swap();
      S_counterClock(1);
      L_clockwise();
      E_clockwise(2);

    }
    else if (whereTo == 14) {
      S_counterClock(1);
      swap();
      S_clockwise();

    }
    else if (whereTo == 15) {
      E_clockwise(2);
      L_clockwise();
      S_clockwise();
      swap();
      S_counterClock(1);
      L_counterClock(1);
      E_clockwise(2);
    }
  }
}

void putting_corner_in_place(int whereTo) {
  Serial.print("\n Ir para: ");
  Serial.print(whereTo);
  if (whereTo < 8) {
    if (whereTo == 0) {
      L_counterClock(2);
      D_counterClock(1);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      D_clockwise();
      L_counterClock(2);

    }
    else if (whereTo == 1) {
      swap();
    }
    else if (whereTo == 2) {
      //BUFFER
    }
    else if (whereTo == 3) {
      L_counterClock(2);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      L_counterClock(2);

    }
    else if (whereTo == 4) {
      B_counterClock(1);
      swap();
      B_clockwise();

    }
    else if (whereTo == 5) {
      L_counterClock(1);
      B_counterClock(1);
      swap();
      B_clockwise();
      L_clockwise();

    }
    else if (whereTo == 6) {
      L_counterClock(2);
      B_counterClock(1);
      swap();
      B_clockwise();
      L_counterClock(2);

    }
    else if (whereTo == 7) {
      L_clockwise();
      B_counterClock(1);
      swap();
      B_clockwise();
      L_counterClock(1);
    }
  }
  else if (whereTo > 15) {
    if (whereTo == 16) {
      B_clockwise();
      L_counterClock(1);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      L_clockwise();
      B_counterClock(1);
    }
    else if (whereTo == 17) {
      L_counterClock(1);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      L_clockwise();

    }
    else if (whereTo == 18) {
      D_counterClock(1);
      B_clockwise();
      swap();
      B_counterClock(1);
      D_clockwise();

    }
    else if (whereTo == 19) {
      D_counterClock(2);
      L_clockwise();
      B_counterClock(2);
      swap();
      B_counterClock(2);
      L_counterClock(1);
      D_counterClock(2);

    }
    else if (whereTo == 20) {
      D_counterClock(1);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      D_clockwise();

    }
    else if (whereTo == 21) {
      D_counterClock(2);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      D_counterClock(2);

    }
    else if (whereTo == 22) {
      D_clockwise();
      B_counterClock(2);
      swap();
      B_counterClock(2);
      D_counterClock(1);

    }
    else if (whereTo == 23) {
      B_counterClock(2);
      swap();
      B_counterClock(2);
    }
  }
  else {
    if (whereTo == 8) {
      L_clockwise();
      D_counterClock(1);
      B_counterClock(2);
      swap();
      B_counterClock(2);
      D_clockwise();
      L_counterClock(1);
    }
    else if (whereTo == 9) {
      //BUFFER
    }
    else if (whereTo == 10) {
      D_clockwise();
      B_clockwise();
      swap();
      B_counterClock(1);
      D_counterClock(1);
    }
    else if (whereTo == 11) {
      L_clockwise();
      B_counterClock(2);
      swap();
      B_counterClock(2);
      L_counterClock(1);

    }
    else if (whereTo == 12) {
     //BUFFER
    }
    else if (whereTo == 13) {
      B_counterClock(2);
      L_clockwise();
      B_counterClock(1);
      swap();
      B_clockwise();
      L_counterClock(1);
      B_counterClock(2);

    }
    else if (whereTo == 14) {
      B_clockwise();
      swap();
      B_counterClock(1);

    }
    else if (whereTo == 15) {
      D_counterClock(1);
      L_clockwise();
      B_counterClock(2);
      swap();
      B_counterClock(2);
      L_counterClock(1);
      D_clockwise();
    }
  }
}

///////////////////////////////////////////////////// Movements planning //////////////////////////////////////////////////////////////////////////

int find_face(char color){
  //char faces_color[] = "wogrby";
  int i;

  for (i = 0; i < 6; i++) {
    if (faces_color[i] == color) {
      break;
    }
  }
  return i;
}

int find_position_middle(int face_1, int face_2){
  int piece[24] = {17, 13, 9, 5, 4, 12, 24, 18, 3, 16, 21, 6, 2, 20, 22, 10, 1, 8, 23, 14, 11, 15, 19, 7};
  int pos2[4];
  int pos1;

  for(int i=0;i<4;i++){
    pos2[i] = (face_2 * 4)+i;
    }
  for(int i=0;i<4;i++){
    pos1 = (face_1 * 4)+i;
    int second = piece[pos1] - 1;
    if((second == pos2[0])||(second == pos2[1])||(second == pos2[2])||(second == pos2[3])){
      break;
      }
    }
  return pos1;
  }

void map_middle(){
     
    int piece[24] = {17, 13, 9, 5, 4, 12, 24, 18, 3, 16, 21, 6, 2, 20, 22, 10, 1, 8, 23, 14, 11, 15, 19, 7};
    int loop_time = 24;
    int buffer_position = 2;
   
    int analizando = buffer_position;
    int analizando_2nd_position = piece[analizando]-1;
    int face_position, face_2nd_position;
    int right_pos;
    char analizando_color;

    bool moves_number_is_even = true;
    int controle = 0;

    while(loop_time > 0){
     
        analizando = buffer_position;// será?

       
        if (piece[buffer_position] == 0) {
              for (int i = controle; i < 24; i++) {
                if (piece[i] != 0){
                  analizando = i;
                  controle = i;
                  break;
                }
              }
        }
       
        analizando_2nd_position = piece[analizando]-1;
       
        analizando_color = middle_colors[analizando];
        face_position = find_face(analizando_color);
        analizando_color = middle_colors[analizando_2nd_position];
        face_2nd_position = find_face(analizando_color);

        right_pos = find_position_middle(face_position, face_2nd_position);

        if(right_pos == analizando){
            //peça no lugar certo e não necessariamente no buffer
              Serial.print("\n peça no lugar certo. ");
              piece[analizando] = 0;
              piece[analizando_2nd_position] = 0;
              loop_time = loop_time - 2;
         
          }else if(right_pos == analizando_2nd_position){
              //peça invertida
              if(analizando != buffer_position){
                  Serial.print("\n Mandando buffer: ");
                  Serial.print(analizando);
                  putting_middle_in_place(analizando);//<<<<<<<<<<
                  if(analizando != 2 && analizando != 8) moves_number_is_even = !moves_number_is_even;
                  Serial.print("\n Desinverte, mandar para: ");
                  Serial.print(analizando_2nd_position);
                  putting_middle_in_place(analizando_2nd_position);//<<<<<<<<<<
                  if(analizando_2nd_position != 2 && analizando_2nd_position != 8) moves_number_is_even = !moves_number_is_even;    
                  piece[analizando] = 0;
                  piece[analizando_2nd_position] = 0;
                  loop_time = loop_time - 2;
             
                  }
             
          }else{
              //peça fora do lugar
              if(analizando == buffer_position){
                  Serial.print("\n Enviando para posicao correta: ");
                  Serial.print(right_pos);
                  putting_middle_in_place(right_pos);//<<<<<<<<<<
                  if(right_pos != 2 && right_pos != 8) moves_number_is_even = !moves_number_is_even;
                 
                  piece[analizando] = 0;
                  piece[analizando_2nd_position] = 0;
                  loop_time = loop_time - 2;
                  buffer_position = right_pos;
                 
                }else{
                      //salvar buffer aintigo?
                      Serial.print("\n Mandando buffer: ");
                      Serial.print(analizando);
                      putting_middle_in_place(analizando);//<<<<<<<<<<
                      if(analizando != 2 && analizando != 8) moves_number_is_even = !moves_number_is_even;
                      Serial.print("\n Mandando para lugar certo: ");
                      Serial.print(right_pos);
                      putting_middle_in_place(right_pos);//<<<<<<<<<<
                      if(right_pos != 2 && right_pos != 8) moves_number_is_even = !moves_number_is_even;
                     
                      piece[analizando] = 0;
                      piece[analizando_2nd_position] = 0;
                      loop_time = loop_time - 2;
                      buffer_position = right_pos;

            }
          }
    Serial.print("\n -----------------------------");      
    }
  if (moves_number_is_even == false) {
    Serial.print("\n Não e par");
    swap();
  }
  Serial.print("fim meios");
}

int find_position_corner(int face_1, int face_2, int face_3){
 
  int piece[24] = {5,14,10,6,1,4,12,19,4,3,16,7,3,2,20,11,2,1,8,15,7,11,15,8};
  int piece2[24] = {18,17,13,9,18,9,21,24,6,13,22,21,10,17,23,22,14,5,24,23,12,16,20,19};
 
  int pos2[4];
  int pos3[4];
  int pos1, retorno;

  for(int i=0;i<4;i++){
    pos2[i] = (face_2 * 4)+i;
    pos3[i] = (face_3 * 4)+i;
    }
   
  for(int i=0;i<4;i++){
    pos1 = (face_1 * 4)+i;
    int second = piece[pos1] - 1;
    int tird = piece2[pos1] - 1;
    for(int j=0; j<4;j++){
      if(second == pos2[j]||tird == pos2[j]){
        for(int k=0;k<4;k++){
          if(tird == pos3[k]||second == pos3[k]){
            retorno = pos1;
            i=4;
            j=4;
            break;
            }
          }
        }
      }
    }
  return retorno;
  }

void map_corners(){
     
    int piece[24] = {5,14,10,6,1,4,12,19,4,3,16,7,3,2,20,11,2,1,8,15,7,11,15,8};
    int piece2[24] = {18,17,13,9,18,9,21,24,6,13,22,21,10,17,23,22,14,5,24,23,12,16,20,19};
    int loop_time = 24;
    int buffer_position = 2;
   
    int analizando = buffer_position;
    int analizando_2nd_position = piece[analizando]-1;
    int analizando_3rd_position = piece2[analizando]-1;
    int face_position, face_2nd_position, face_3rd_position;
    int right_pos;
    char analizando_color;

    int controle = 0;

    while(loop_time > 0){
     
        analizando = buffer_position;// será?

       
        if (piece[buffer_position] == 0) {
              for (int i = controle; i < 24; i++) {
                if (piece[i] != 0){
                  analizando = i;
                  controle = i;
                  break;
                }
              }
        }
       
        analizando_2nd_position = piece[analizando]-1;
        analizando_3rd_position = piece2[analizando]-1;
       
        analizando_color = corner_colors[analizando];
        face_position = find_face(analizando_color);
        analizando_color = corner_colors[analizando_2nd_position];
        face_2nd_position = find_face(analizando_color);
        analizando_color = corner_colors[analizando_3rd_position];
        face_3rd_position = find_face(analizando_color);

        right_pos = find_position_corner(face_position, face_2nd_position, face_3rd_position);

        if(right_pos == analizando){
            //peça no lugar certo e não necessariamente no buffer
              Serial.print("\n peça no lugar certo. ");
              piece[analizando] = 0;
              piece[analizando_2nd_position] = 0;
              piece[analizando_3rd_position] = 0;
              loop_time = loop_time - 3;
         
          }else if(right_pos == analizando_2nd_position||right_pos == analizando_3rd_position){
              //peça invertida
              if(analizando != buffer_position){
                  Serial.print("\n Mandando buffer: ");
                  Serial.print(analizando);
                  putting_corner_in_place(analizando);//<<<<<<<<<<
                  Serial.print("\n Desinverte, mandar para: ");
                  Serial.print(right_pos);
                  putting_corner_in_place(right_pos);//<<<<<<<<<<                        
                  piece[analizando] = 0;
                  piece[analizando_2nd_position] = 0;
                  piece[analizando_3rd_position] = 0;
                  loop_time = loop_time - 3;
             
                  }
             
          }else{
              //peça fora do lugar
              if(analizando == buffer_position){
                  Serial.print("\n Enviando para posicao correta: ");
                  Serial.print(right_pos);
                  putting_corner_in_place(right_pos);//<<<<<<<<<<                  
                  piece[analizando] = 0;
                  piece[analizando_2nd_position] = 0;
                  piece[analizando_3rd_position] = 0;
                  loop_time = loop_time - 3;
                  buffer_position = right_pos;
                 
                }else{
                      //salvar buffer aintigo?
                      Serial.print("\n Mandando buffer: ");
                      Serial.print(analizando);
                      putting_corner_in_place(analizando);//<<<<<<<<<<
                      Serial.print("\n Mandando para lugar certo: ");
                      Serial.print(right_pos);
                      putting_corner_in_place(right_pos);//<<<<<<<<<<
                     
                      piece[analizando] = 0;
                      piece[analizando_2nd_position] = 0;
                      piece[analizando_3rd_position] = 0;
                      loop_time = loop_time - 3;
                      buffer_position = right_pos;
           
            }
          }
    Serial.print("\n -------------------------- ");

    }

  Serial.print("fim");
}

/////////////////////////////////////////////////////////// Servo Moves /////////////////////////////////////////////////////////////////

void swap() {
  R_clockwise();
  U_clockwise();
  R_counterClock(1);

  F_counterClock(1);

  R_clockwise();
  U_clockwise();
  R_counterClock(1);

  U_counterClock(1);
  R_counterClock(1);
  F_clockwise();
  R_counterClock(2);

  U_counterClock(1);
  R_counterClock(1);
  U_counterClock(1);

}

////////////////////////////
void U_clockwise() {
  turn_clockwise();
  R_clockwise();
  turn_counterClock();
}

void U_counterClock(int times) {
  turn_clockwise();
  R_counterClock(times);
  turn_counterClock();
}

void D_clockwise() {
  turn_clockwise();
  L_clockwise();
  turn_counterClock();
}

void D_counterClock(int times) {
  turn_clockwise();
  L_counterClock(times);
  turn_counterClock();
}

void E_clockwise(int times) {
  turn_front_2up();
  if(times == 1){
    F_counterClock(1);
    B_clockwise();
  }else{
    F_counterClock(2);
    B_counterClock(2);
    turn_clockwise();
  }
  turn_clockwise();
  turn_up_2front();
}

void E_counterClock(int times) {
  turn_front_2up();
  turn_counterClock();
  if(times == 1){
    B_counterClock(1);
    F_clockwise();
  }else{
    turn_counterClock();
    B_counterClock(2);
    F_counterClock(2);
    }
  turn_up_2front();
}

void Lw_clockwise() {
  R_clockwise();
  turn_up_2front();
}

void Lw_counterClock(int times) {
  turn_front_2up();
  if(times == 1){
    R_counterClock(1);
  }else{
    turn_front_2up();
    R_counterClock(1);
    }
}

void S_clockwise() {
  F_counterClock(1);
  B_clockwise();
  turn_clockwise();
}

void S_counterClock(int times) {
  if(times == 1){
    F_clockwise();
    B_counterClock(1);
  }else{
    F_counterClock(2);
    B_counterClock(2);
    turn_counterClock();
    }
  turn_counterClock();
}

void turn_up_2front() {
  servoPos_R.write(posZero);
  delay(500);
  servo_R.write(claw_R90);
  delay(500);
  servoPos_R.write(base_90_R);
  delay(500);
 
  servoPos_B.write(posZero);
  servoPos_F.write(posZero);
  delay(500);

  servo_L.write(claw_L90);
  servo_R.write(posZero);
  delay(500);

  servoPos_B.write(base_90_FB);
  servoPos_F.write(base_90_FB);
  delay(500);

  servoPos_L.write(posZero);
  delay(500);
  servo_L.write(posZero);
  delay(500);
  servoPos_L.write(base_90_L);
  delay(600);
 
}

void turn_front_2up() {
  servoPos_L.write(posZero);
  delay(500);
  servo_L.write(claw_L90);
  delay(500);
  servoPos_L.write(base_90_L);
  delay(500);

  servoPos_B.write(posZero);
  servoPos_F.write(posZero);
  delay(500);

  servo_R.write(claw_R90);
  servo_L.write(posZero);
  delay(500);

  servoPos_B.write(base_90_FB);
  servoPos_F.write(base_90_FB);
  delay(500);

  servoPos_R.write(posZero);
  delay(500);
  servo_R.write(posZero);
  delay(500);
  servoPos_R.write(base_90_R);
  delay(600);
}

void turn_clockwise() {
  servoPos_B.write(posZero);
  delay(500);
  servo_B.write(claw_B90);
  delay(500);
  servoPos_B.write(base_90_FB);
  delay(500);

  servoPos_R.write(posZero);
  servoPos_L.write(posZero);
  delay(500);

  servo_F.write(claw_F90);
  servo_B.write(posZero);
  delay(500);

  servoPos_R.write(base_90_R);
  servoPos_L.write(base_90_L);
  delay(500);

  servoPos_F.write(posZero);
  delay(500);
  servo_F.write(posZero);
  delay(500);
  servoPos_F.write(base_90_FB);
  delay(600);
}

void turn_counterClock() {
  servoPos_F.write(posZero);
  delay(500);
  servo_F.write(claw_F90);
  delay(500);
  servoPos_F.write(base_90_FB);
  delay(500);

  servoPos_R.write(posZero);
  servoPos_L.write(posZero);
  delay(500);

  servo_B.write(claw_B90);
  servo_F.write(posZero);
  delay(500);

  servoPos_R.write(base_90_R);
  servoPos_L.write(base_90_L);
  delay(500);

  servoPos_B.write(posZero);
  delay(500);
  servo_B.write(posZero);
  delay(500);
  servoPos_B.write(base_90_FB);
  delay(600);
}

void F_clockwise() {
  servo_F.write(claw_F90);
  delay(500);
  servoPos_F.write(posZero);
  delay(500);
  servo_F.write(posZero);
  delay(500);
  servoPos_F.write(base_90_FB);
  delay(600);
}

void F_counterClock(int times) {
  int aux;
  if(times == 1) aux = claw_F90;
  else aux = claw_F180;
 
  servoPos_F.write(posZero);
  delay(500);
  servo_F.write(aux);
  delay(500);
  servoPos_F.write(base_90_FB);
  delay(500);
  servo_F.write(posZero);
  delay(600);
}

void B_clockwise() {
  servo_B.write(claw_B90);
  delay(500);
  servoPos_B.write(posZero);
  delay(500);
  servo_B.write(posZero);
  delay(500);
  servoPos_B.write(base_90_FB);
  delay(600);
}

void B_counterClock(int times) {
  int aux;
  if(times == 1) aux = claw_B90;
  else aux = claw_B180;
 
  servoPos_B.write(posZero);
  delay(500);
  servo_B.write(aux);
  delay(500);
  servoPos_B.write(base_90_FB);
  delay(500);
  servo_B.write(posZero);
  delay(600);
}

void L_clockwise() {
  servo_L.write(claw_L90);
  delay(500);
  servoPos_L.write(posZero);
  delay(500);
  servo_L.write(posZero);
  delay(500);
  servoPos_L.write(base_90_L);
  delay(600);
}

void L_counterClock(int times) {
  int aux;
  if(times == 1) aux = claw_L90;
  else aux = claw_L180;
 
  servoPos_L.write(posZero);
  delay(500);
  servo_L.write(aux);
  delay(500);
  servoPos_L.write(base_90_L);
  delay(500);
  servo_L.write(posZero);
  delay(600);
}

void R_clockwise() {
  servo_R.write(claw_R90);
  delay(500);
  servoPos_R.write(posZero);
  delay(500);
  servo_R.write(posZero);
  delay(500);
  servoPos_R.write(base_90_R);
  delay(600);
}

void R_counterClock(int times) {
  int aux;
  if(times == 1) aux = claw_R90;
  else aux = claw_R180;
 
  servoPos_R.write(posZero);
  delay(500);
  servo_R.write(aux);
  delay(500);
  servoPos_R.write(base_90_R);
  delay(500);
  servo_R.write(posZero);
  delay(600);

}

void recebe_cubo() {

  /*Posição para receber o cubo
     servos de posição: frente e esquerda no zero, direita e fundo no 90
  */
  servoPos_F.write(posZero);
  servoPos_L.write(posZero);
  delay(2000);

  servoPos_B.write(posZero);
  servoPos_R.write(posZero);
  delay(2000);

  servoPos_B.write(base_90_FB);
  servoPos_R.write(base_90_R);
  delay(4000);

  //segura o cubo
  servoPos_F.write(base_90_FB);
  servoPos_L.write(base_90_L);
  delay(2000);
}

void solta_cubo() {

  servoPos_F.write(posZero);
  servoPos_L.write(posZero);
  delay(2000);

  servoPos_B.write(posZero);
  servoPos_R.write(posZero);
  delay(2000);

}

void loop() {
}