#include "fichier.h"
#include "serveursynergie.h"
#include <QTextOStream>
#include "Console/console.h"
#include "Reseau/Paquets/paquetinsertiontexte.h"

Fichier::Fichier(int id, QString chemin, QObject *parent) :
    QObject(parent)
{
    m_ID = id;
    m_Chemin = chemin;
    m_Clients = new QList<Client*>();
    m_Charge = false;
}

void Fichier::Sauvegarder()
{
    QTextStream stream(m_Fichier);
    stream << m_Contenu;
    Console::getInstance()->Imprimer(m_Chemin + " sauvegardé");
}

void Fichier::ChargerContenu()
{
    if (!m_Charge) {
        m_Fichier = new QFile("Projets/" + ServeurSynergie::getInstance()->getProjet() + "/" + m_Chemin); // Corriger ici
        m_Fichier->open(QFile::ReadWrite);
        QTextStream* stream = new QTextStream(m_Fichier);
        m_Contenu = stream->readAll();
        m_Charge = true;
        m_Fichier->close();
        m_Fichier->open(QFile::WriteOnly|QFile::Truncate);
    }
}

void Fichier::DechargerContenu()
{
    Sauvegarder();

    if (m_Charge) {
        m_Fichier->close();
        m_Contenu = QString::null;
        m_Charge = false;
    }
}

void Fichier::AjouterClient(Client *client)
{
    if (m_Clients->count() == 0) {
        ChargerContenu();
    }

    m_Clients->append(client);
}

void Fichier::EnleverClient(Client *client)
{
    m_Clients->removeOne(client);

    if (m_Clients->count() == 0) {
        DechargerContenu();
    }
}

void Fichier::InsererTexte(QString texte, int position, Client* auteur)
{
    m_Contenu.insert(position, texte);

    Client* client;
    foreach (client, *m_Clients) {
        if (client != auteur) {
            client->EnvoyerPaquet(new PaquetInsertionTexte(this, texte, position));
        }
    }
}

QString Fichier::getChemin()
{
    return m_Chemin;
}

int Fichier::getID()
{
    return m_ID;
}

QString* Fichier::getContenu()
{
    return &m_Contenu;
}

QList<Client*>* Fichier::getClients()
{
    return m_Clients;
}
