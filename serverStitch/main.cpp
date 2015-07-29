#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>				//g++ arq.cpp -o arq -pthread -std=c++11
                                // ls -l *.cpp|wc -l
                                // vips im_vips2tiff land_shallow_topo_8192.tif land.tif:deflate,tile:256x256,pyramid

#include <thread>
#include <unistd.h>

using namespace std;
int nFotos;

void writeFile(char* file);
int readFile(char* file);
void loop()
{
    while(true){
        
        //writeFile("/home/mogai/ownCloud/eot.txt");
        nFotos = readFile( "/home/mogai/ownCloud/eot.txt");
        if( nFotos > 0 ){
            
            cout << endl;
            //ve qual a ultima corrida
            string corridaString;
            ifstream infile;
            infile.open("/home/mogai/uscan/photos/corrida.txt");
            infile >> corridaString;
            int corrida = atoi(corridaString.c_str());
            cout << "Corrida numero: " << corrida << endl;
            
            //cria novo folder p corrida e move fotos.png e eot.txt
            string comando;
            comando = "mkdir /home/mogai/uscan/photos/"+ to_string(corrida);
            system(comando.c_str());
            comando = "mv /home/mogai/ownCloud/*.png /home/mogai/uscan/photos/"+ to_string(corrida);
            system(comando.c_str());
            comando = "mv /home/mogai/ownCloud/eot.txt /home/mogai/uscan/photos/"+ to_string(corrida);
            system(comando.c_str());
            
            //atualiza corrida
            ofstream outfile;
            outfile.open("/home/mogai/uscan/photos/corrida.txt");
            outfile << corrida + 1 << endl;  //incrementa corrida
            outfile.close();
            
            //monta comando e executa stitcher
            string nomeFoto;
            string pathFotos;
            string folderOutput;
            folderOutput = "--output /home/mogai/uscan/photos/"+to_string(corrida)+"/stitch.png ";
            for(int i=0; i< nFotos; i++){
                nomeFoto = "/home/mogai/uscan/photos/"+to_string(corrida)+"/"+to_string(i)+".png ";
                pathFotos = pathFotos + nomeFoto;
            }
            
            comando = "(cd /home/mogai/uscan/stitcher && ./stitch ";
            comando = comando + pathFotos + folderOutput + ")";
            cout << comando << endl;
            system(comando.c_str());
            
            comando = "cp /home/mogai/uscan/photos/"+to_string(corrida)+"/stitch.png /var/www/html/imagens/stitch"+to_string(corrida)+".png";
            system(comando.c_str());
        }
        sleep(3);
    }
}

int main ()
{
    //system("ls -l /home/mogai/ownCloud/*.png|wc -l");
    //cout <<"n"<< c<< endl;
    thread routine(loop);
    routine.join();
    return 0;
}

int readFile(char* file){
    
    FILE *f = fopen(file, "r+");
    int x;
    if(f != NULL){
        string data;
        
        ifstream infile;
        infile.open(file);
        
        infile >> data;
        int x = atoi(data.c_str());
        cout <<"ID-> "<< x << endl;
        
        infile >> data;
        x = atoi(data.c_str());
        cout <<"Fotos-> "<< x << endl;
        /*
         infile >> data;
         x = atoi(data.c_str());
         cout <<"Passo Y-> "<< x << endl;
         
         infile >> data;
         x = atoi(data.c_str());
         cout <<"Tempo-> "<< x << endl; 
         */
        infile.close();
        fclose(f);
        return x;
    }
    else
    {	x = -1;
        cout << "Waiting transmission.."<<endl;
        return x;
    }
}

void writeFile(char* file){
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    // convert to string form
    char* dt = ctime(&now);
    char data[100];
    ofstream outfile;
    
    outfile.open(file);
    
    strcpy(data, "1 5 5 300");
    outfile << data << endl;
    outfile << "Date: " << ltm->tm_mday << " " << 1 + ltm->tm_mon << " " << 1900 +ltm->tm_year << endl;
    outfile << "Time: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << 1 + ltm->tm_sec << endl;
    outfile.close();
}
