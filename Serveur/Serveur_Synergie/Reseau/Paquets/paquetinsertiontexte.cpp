#include "paquetinsertiontexte.h"

PaquetInsertionTexte::PaquetInsertionTexte(Fichier* fichier, QString texte, int position) :
    BasePaquetServeur((qint8)5)
{
    *m_Stream << fichier->getID();
    *m_Stream << texte;
    *m_Stream << position;
}