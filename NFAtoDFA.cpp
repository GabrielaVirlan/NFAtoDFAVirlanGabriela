#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct tranzitie{
    string simbol;
    string stare;
};
struct stare{
    string stare;
    vector<tranzitie> tranzitii;

};
struct automat{
    string finalStates;
    vector<stare> stari;
};

void citire_automat(automat& deCitit)
{
    ifstream in("NFA.in");
    int nrStari,nrStariFinale;
    in>>nrStari;
    deCitit.stari.resize(nrStari);
    in>>nrStariFinale;
    for(int i=0;i<nrStariFinale;i++)
    {
        char stareFinala;
        in>>stareFinala;
        deCitit.finalStates+=stareFinala;
    }
    int stareCurenta;
    char simbol,stareTinta;
    while(in>>stareCurenta>>stareTinta>>simbol)
    {
        deCitit.stari[stareCurenta].stare=(char)(stareCurenta+'0');
        tranzitie tranzitieTemp;
        tranzitieTemp.simbol=simbol;
        tranzitieTemp.stare=stareTinta;
        deCitit.stari[stareCurenta].tranzitii.push_back(tranzitieTemp);
    }
}

void afisare_automat(automat deAfisat)
{
    cout<<"AUTOMAT:"<<endl;
    for(int i=0;i<deAfisat.stari.size();i++)
    {
        cout<<deAfisat.stari[i].stare<<":"<<endl;
        for(int j=0;j<deAfisat.stari[i].tranzitii.size();j++)
            cout<<deAfisat.stari[i].tranzitii[j].stare<<" "<<deAfisat.stari[i].tranzitii[j].simbol<<endl;
    }
    cout<<"STARI FINALE:"<<endl;
    cout<<deAfisat.finalStates;
}
automat conversie_NFS_DFS(automat deConvertit)
{
    automat DFS;
    stare aux;
    aux.stare=deConvertit.stari[0].stare;
    DFS.stari.push_back(aux);

    for(int i=0;i<DFS.stari.size();i++) //stare curenta DFS (compunere stari NFA)
    {
        for(int j=0;j<DFS.stari[i].stare.size();j++) //stare curenta atomica
        {
            for(int k=0;k<deConvertit.stari.size();k++)  //cauta starea atomica in NFA
                if(deConvertit.stari[k].stare[0]==DFS.stari[i].stare[j])
                {
                    for(int m=0;m<deConvertit.stari[k].tranzitii.size();m++)  //parcurge tranzitiile starii atomice
                    {
                        //adauga tranzitiile starii atomice in tranzitiile DFS
                        int gasit=0;
                        for(int n=0;n<DFS.stari[i].tranzitii.size();n++) //cauta tranzitia cu simbolul curent in starea actuala din DFS
                        {
                            if(!DFS.stari[i].tranzitii[n].simbol.compare(deConvertit.stari[k].tranzitii[m].simbol)) //daca gaseste, adauga starea atomica in tranzitia cu simbolul curent
                            {
                                if(DFS.stari[i].tranzitii[n].stare.find(deConvertit.stari[k].tranzitii[m].stare)==string::npos)
                                    DFS.stari[i].tranzitii[n].stare+=deConvertit.stari[k].tranzitii[m].stare;
                                gasit=1;
                            }
                        }
                        if(gasit==0) //daca nu gaseste, adauga o tranzitie cu simbolul curent la starea atomica gasita
                        {

                            DFS.stari[i].tranzitii.push_back(deConvertit.stari[k].tranzitii[m]);
                        }
                    }
                }
        }
        for(int j=0;j<DFS.stari[i].tranzitii.size();j++) //adauga starile nou gasite in DFS pentru a fi procesate (daca nu exista)
        {
            bool add=1;
            for(int k=0;k<DFS.stari.size();k++)
                if(!DFS.stari[i].tranzitii[j].stare.compare(DFS.stari[k].stare))
                    add=0;
            if(add)
            {
                stare temp;
                temp.stare=DFS.stari[i].tranzitii[j].stare;
                DFS.stari.push_back(temp);
            }
        }

    }
    for(int i=0;i<DFS.stari.size();i++)
    {
        bool stFin=0;
        for(int j=0;j<deConvertit.finalStates.size();j++)
            if(DFS.stari[i].stare.find(deConvertit.finalStates[j])!=string::npos)
                stFin=1;
        if(stFin)
        {
            DFS.finalStates+=DFS.stari[i].stare;
            DFS.finalStates+=' ';
        }
    }

    return DFS;
}

int main()
{
    automat X;
    citire_automat(X);
    automat DFS=conversie_NFS_DFS(X);
    afisare_automat(DFS);
}
