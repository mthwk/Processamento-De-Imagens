#include <iostream>
#include <cmath>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;
const int MAX= 550;
const int sMAX=51;

struct Img
{
    int nli; //numero de linhas da imagem;
    int nco; //numero de colunas da imagem;
    int m[MAX][MAX][3]; // cores do pixel;
};

struct Pixel{
    int red;
    int green;
    int blue;
};

bool abre_img(char nome[], Img& img);
bool salva_img(char nome[], Img img);
void binariza(Img img_in, Img& img_out, int limiar);
void mascara_cons_dir(Img img, Img& img_out);
void mascara_cons_esq(Img img, Img& img_out);
void mascara_cons_sup(Img img, Img& img_out);
void mascara_cons_inf(Img img, Img& img_out);
void mascara_xadrez(Img img, Img& img_out);
void mascara_branca(Img img, Img& img_out);
void binarizacao_mascara(Img img_in, Img& img_out,Img& masc, float limiar);
void rotaciona(Img img_in, Img& img_out);
void diminui_tamanho(Img img_in, Img& img_out);
void preenche_regiao(Img& img, int l, int c, Pixel cor_orig, Pixel cor_nova);

int main()
{
    int x=72,y=0,num_masc,l,c;
    float limiar;
    char simg[sMAX],sppm[]=".ppm", nome_saida[sMAX];
    Img imagem;
    Img imagem_de_saida;
    Img masc;
    Pixel cor_orig,cor_nova;
    bool outra=false,fim=true;
    while(x!=0){
        if(outra==false)
        {
            cout<<"Digite o nome da imagem que voce quer editar:"<<endl;
            cin.getline(simg,sMAX);
            strcat(simg,sppm);

            while(!abre_img(simg,imagem))
            {
                cout<<"Ops ocorreu algo errado, insira o nome do arquivo novamente e/ou verifique se a imagem esta no formato ppm."<<endl;
                cin.getline(simg,sMAX);
                strcat(simg,sppm);
            }
        }
        else
        {
            strcpy(simg,nome_saida);
            while(!abre_img(simg,imagem))
            {
                cout<<"Ops ocorreu algo errado, insira o nome do arquivo novamente e/ou verifique se a imagem esta no formato ppm."<<endl;
                cin.getline(simg,sMAX);
            }
        }
        cout<<"\nDigite 1 caso queira deixar sua imagem preto e branco.\nDigite 2 para rotacionar a imagem no sentido horario. \nDigite 3 para reduzir o tamanho da imagem. \nDigite 4 para preencher regioes com determinadas cores. \nDigite 0 para sair do programa."<<endl;
        cin>>x;
        while(x<0 || x>4){
            cout << "O valor inserido foi invalido, por favor digite outra opcao." << endl;
            cin>>x;
        }
        switch(x){
        case 0:
            fim=false;
            break;

        case 1://  Chamando a binariza.
            cout<<"\nVoce selecionou a opcao binarizar em preto e branco."<<endl;
            cout << "\nEscolha uma opcao de mascara abaixo:\n1-Sem mascara.\n2-Mascara que conserva o lado esquerdo.\n3-Mascara que conserva o lado direito.\n4-Mascara que conserva o lado superior.\n5-Mascara que conserva o lado inferior.\n6-Mascara do tipo xadrez."<<endl;
            cin >> num_masc;
            while(num_masc < 1 || num_masc > 6){
                cout << "O valor digitado foi invalido, por favor digite um valor entre 1 e 6." << endl;
                cin >> num_masc;
            }

            switch(num_masc){
        case 1:
            mascara_branca(imagem,masc);
            break;
        case 2:
            mascara_cons_esq(imagem,masc);
            break;
        case 3:
            mascara_cons_dir(imagem,masc);
            break;
        case 4:
            mascara_cons_sup(imagem,masc);
            break;
        case 5:
            mascara_cons_inf(imagem,masc);
            break;
        case 6:
            mascara_xadrez(imagem,masc);
            break;
            }

            cout << endl;
            cout << "Digite um valor entre 0 e 255 para o limiar: \n";
            cin >> limiar;
            while(limiar < 0 || limiar > 255){
                cout << "O valor digitado foi invalido, por favor digite um valor entre 0 e 255" << endl;
                cin >> limiar;
            }
            cin.ignore();

            binarizacao_mascara(imagem,imagem_de_saida,masc,limiar);

            cout << endl;
            cout << "Digite o nome do novo arquivo: \n";
            cin.getline(nome_saida,sMAX);
            strcat(nome_saida,sppm);
            salva_img(nome_saida,imagem_de_saida);
            break;
        case 2:   // Chamando a rotaciona.

            cout << endl;
            cout << "Voce escolheu a opcao de rotacionar no sentido horario. \n";
            cout << endl;
            rotaciona(imagem,imagem_de_saida);
            cout << "Digite o nome do novo arquivo: \n";
            cin.ignore();
            cin.getline(nome_saida,sMAX);
            strcat(nome_saida,sppm);
            salva_img(nome_saida,imagem_de_saida);
            break;
        case 3:
            cout << endl;
            cout << "Voce escolheu a opcao de diminuir a imagem. \n";
            cout << endl;
            diminui_tamanho(imagem,imagem_de_saida);
            cout << "Digite o nome do novo arquivo: \n";
            cin.ignore();
            cin.getline(nome_saida,sMAX);
            strcat(nome_saida,sppm);
            salva_img(nome_saida,imagem_de_saida);
        case 4:
            cout << endl;
            cout << "Voce escolheu a opcao de preencher regiao. \n";
            cout << endl;
            imagem_de_saida=imagem;
            cout << "Insira a o numero da coluna e da linha, respectivamente,do ponto em que voce deseja aplicar o preenchimento."<<endl;
            cin >> c >> l;
            while(c>=imagem_de_saida.nco||c<0||l>=imagem_de_saida.nli||l<0){
                cout << "insira um ponto valido"<<endl;
                cin >> c >>l;
            }
            cor_orig.red=imagem_de_saida.m[c][l][0];
            cor_orig.green=imagem_de_saida.m[c][l][1];
            cor_orig.blue=imagem_de_saida.m[c][l][2];
            cout<< "insira a nova cor, vermelho, verde, azul, respectivamente."<<endl;
            cin>> cor_nova.red>>cor_nova.green>>cor_nova.blue;
            while(cor_nova.red == cor_orig.red && cor_nova.green == cor_orig.green && cor_nova.blue == cor_orig.blue){
                cout<<"essa cor ja tem no ponto, por favor escolha outra cor"<<endl;
                cin>> cor_nova.red>>cor_nova.green>>cor_nova.blue;
            }
            preenche_regiao(imagem_de_saida,l,c,cor_orig,cor_nova);
                        cout << "Digite o nome do novo arquivo: \n";
            cin.ignore();
            cin.getline(nome_saida,sMAX);
            strcat(nome_saida,sppm);
            salva_img(nome_saida,imagem_de_saida);
            break;
        }
        if(fim==true){
        cout<<endl;
        cout<<"Se deseja continuar com a imagem que voce alterou digite 1, caso deseja alterar outra imagem digite 2.\nSe desejar sair aperta qualquer outra tecla.\n"<<endl;
        cin>>y;
        cin.ignore();
        if(y==1){
            outra=true;
        }
        else if(y==2){
            outra=false;
        }
        else
            x=0;
        }
    }
    return 0;
}

bool abre_img(char nome[], Img& img)
{
    fstream arq;
    char s[sMAX],p[]="P3";
    int i,j,k,ma;
    arq.open(nome);
    if(arq.is_open())
    {
        arq>>s;
        if(strcmp(s,p)==0)
        {
            arq>>img.nco>>img.nli;
            arq>>ma;
            for(i=0; i<img.nli; i++)
            {
                for(j=0; j<img.nco; j++)
                {
                    for(k=0; k<3; k++)
                    {
                        arq>>img.m[i][j][k];
                    }
                }
            }
            arq.close();
            return true;
        }
        else
        {
            arq.close();
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool salva_img(char nome[], Img img)
{
    ofstream arq_saida;
    char P3[] = "P3";
    int i,j,k, ma = 255;
    arq_saida.open(nome);

    if(arq_saida.is_open())
    {
        arq_saida << P3 << endl;
        arq_saida << img.nco << " " << img.nli << endl;
        arq_saida << ma << endl;
        for(i=0; i<img.nli; i++)
        {
            for(j=0; j<img.nco; j++)
            {
                for(k=0; k<3; k++)
                {
                    arq_saida << img.m[i][j][k] << " ";
                }
            }
            arq_saida << endl;
        }
        cout << endl;
        cout << "A imagem foi salva corretamente." << endl;
        cout<<endl;
        return true;
    }
    else
    {
        cout << "A imagem nao pode ser salva." << endl;
        return false;
    }
}

void binariza(Img img_in, Img& img_out, int limiar)
{
    int i, j;
    float media_pixel;
    img_out = img_in;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            media_pixel = (img_out.m[i][j][0] + img_out.m[i][j][1] + img_out.m[i][j][2])/3;
            if(media_pixel >= limiar)
            {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
            }
            else
            {
                img_out.m[i][j][0] = 0;
                img_out.m[i][j][1] = 0;
                img_out.m[i][j][2] = 0;
            }
            media_pixel = 0;
        }
    }
}
void mascara_branca(Img img, Img& img_out)
{
    img_out = img;
    int i, j;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
        }
    }
}
void mascara_cons_esq(Img img, Img& img_out)
{
    img_out = img;
    float divide_masc = img_out.nco/2;
    int i, j;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            if(j < divide_masc)
            {
                img_out.m[i][j][0] = 0;
                img_out.m[i][j][1] = 0;
                img_out.m[i][j][2] = 0;
            }
            else
            {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
            }
        }
    }

}
void mascara_cons_dir(Img img, Img& img_out)
{
    img_out = img;
    float divide_masc = img_out.nco/2;
    int i, j;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            if(j < divide_masc)
            {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
            }
            else
            {
                img_out.m[i][j][0] = 0;
                img_out.m[i][j][1] = 0;
                img_out.m[i][j][2] = 0;
            }
        }
    }
}
void mascara_cons_sup(Img img, Img& img_out)
{
    img_out = img;
    float divide_masc = img_out.nli/2;
    int i, j;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            if(i < divide_masc)
            {
                img_out.m[i][j][0] = 0;
                img_out.m[i][j][1] = 0;
                img_out.m[i][j][2] = 0;
            }
            else
            {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
            }
        }
    }
}
void mascara_cons_inf(Img img, Img& img_out)
{
    img_out = img;
    float divide_masc = img_out.nli/2;
    int i, j;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            if(i < divide_masc)
            {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
            }
            else
            {
                img_out.m[i][j][0] = 0;
                img_out.m[i][j][1] = 0;
                img_out.m[i][j][2] = 0;
            }
        }
    }
}
void mascara_xadrez(Img img, Img& img_out)
{
    img_out = img;
    float divide_lin = img_out.nli/2;
    float divide_col = img_out.nco/2;
    int i, j;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            if((i<=divide_lin && j<=divide_col) || (i>divide_lin && j>divide_col))
            {
                img_out.m[i][j][0] = 255;
                img_out.m[i][j][1] = 255;
                img_out.m[i][j][2] = 255;
            }
            else
            {
                img_out.m[i][j][0] = 0;
                img_out.m[i][j][1] = 0;
                img_out.m[i][j][2] = 0;
            }
        }
    }
}
void binarizacao_mascara(Img img_in, Img& img_out,Img& masc, float limiar)
{
    int i, j, k;
    float media_pixel;
    img_out = img_in;

    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            if(masc.m[i][j][0] == 255 && masc.m[i][j][1] == 255 && masc.m[i][j][2] == 255)
            {
                media_pixel = (img_out.m[i][j][0] + img_out.m[i][j][1] + img_out.m[i][j][2])/3;
                if(media_pixel >= limiar)
                {
                    img_out.m[i][j][0] = 255;
                    img_out.m[i][j][1] = 255;
                    img_out.m[i][j][2] = 255;
                }
                else
                {
                    img_out.m[i][j][0] = 0;
                    img_out.m[i][j][1] = 0;
                    img_out.m[i][j][2] = 0;
                }
                media_pixel = 0;
            }
        }
    }
}
void rotaciona(Img img_in, Img& img_out)
{
    img_out = img_in;
    int i,j,k,aux;
    aux=img_out.nli;
    img_out.nli=img_out.nco;
    img_out.nco=aux;
    for(i=0; i<img_out.nli; i++)
    {
        for(j=0; j<img_out.nco; j++)
        {
            for(k=0; k<3; k++)
            {
                img_out.m[i][j][k] = img_in.m[img_in.nli-1-j][i][k];
            }
        }
    }
}
void diminui_tamanho(Img img_in, Img& img_out){
    int novanl, novanc, i, j, l=0,c=0, k;

    if(img_in.nli % 2 != 0){
        novanl = (img_in.nli+1)/2;
    } else {
        novanl = (img_in.nli/2);
    }
    if(img_in.nco % 2 != 0){
        novanc = (img_in.nco+1)/2;
    } else {
        novanc = (img_in.nco/2);
    }
    img_out.nli = novanl;
    img_out.nco = novanc;

    for(i=0;i<img_in.nli;i++){
        if(i%2 == 0){
            for(j=0;j<img_in.nco;j++){
                if(j%2==0){
                    for(k=0;k<3;k++){
                        img_out.m[i/2][j/2][k] = img_in.m[i][j][k];
                    }
                }
            }
        }
    }

}
void preenche_regiao(Img& img, int l, int c, Pixel cor_orig, Pixel cor_nova){
    if(img.m[l][c][0]==cor_orig.red&&img.m[c][l][1]==cor_orig.green&&img.m[c][l][2]==cor_orig.blue){
        img.m[l][c][0]= cor_nova.red;
        img.m[l][c][1]= cor_nova.green;
        img.m[l][c][2]= cor_nova.blue;
        if(l+1<img.nli){
            preenche_regiao(img,l+1,c,cor_orig,cor_nova);
        }
        if(l-1>=0){
            preenche_regiao(img,l-1,c,cor_orig,cor_nova);
        }
        if(c+1<img.nco){
            preenche_regiao(img,l,c+1,cor_orig,cor_nova);
        }
        if(c-1>=0){
            preenche_regiao(img,l,c-1,cor_orig,cor_nova);
        }
    }
}
