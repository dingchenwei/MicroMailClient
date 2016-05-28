#ifndef POP3CLIENT_H
#define POP3CLIENT_H
#include <iostream>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/POP3ClientSession.h>
#include <QDateTime>
#include <QString>
#include <QSharedPointer>
#include <string>
#include <vector>
#include "Account.h"
#include "ReceiveMailClient.h"
#include "MailBody.h"
#include "Attachment.h"

class POP3Client : public ReceiveMailClient{
private:
    typedef QSharedPointer<Poco::Net::POP3ClientSession> SESSION_PTR;
public:

    POP3Client(QString host,
               QString port = QString::number (Poco::Net::POP3ClientSession::POP3_PORT)) {
        this->_session = SESSION_PTR::create(host.toStdString ());
        this->_host = host;
        this->_port = port;
    }


    bool login(QString _user, QString _passwd) override{
        try{
            _session->login(_user.toStdString (), _passwd.toStdString ());
            return true;
        }
        catch(Poco::Net::POP3Exception & e){
            return false;
        }
    }

    void getMailBodies(QList<MAILBODY_PTR> & result){
        POP3ClientSession::MessageInfoVec messages;
        _session.listMessages(messages);
        for(auto i = messages.begin(); i != messages.end(); ++i) {
            MailMessage message;
            _session.retrieveMessage((*i).id, message);
            MAILBODY_PTR newMail = MAILBODY_PTR::create(QString::fromUtf8 (message.getSubject());
            newMail->setContent(QString::fromUtf8 (message.getContent));
            newMail->setSender(QString::fromUtf8 (message.getSender));
            newMail->addRecipient (QString::fromUtf8 (message.Recipients);
            newMail->setIsread (false);
            result.push_back (newMail);
        }
    }

    void DeleteMail (const QList<int> & ids)override{
        return 0;
    }

    void setTimeout (int val)override{
        timeout = Poco::Timespan(0,0,0,0,1000 * val);  //该构造函数默认最小是微妙(第五个参数),前四个:day,hour,minute,second,这里的延时我们需要以毫秒计
        _session->setTimeout(timeout);
    }

    int getTimeout ()override{
        return _session->getTimeout().milliseconds();
    }

    ~POP3Client(){

    }

private:
     Poco::Net::POP3ClientSession::MessageInfoVec _mailList;
     SESSION_PTR _session;
};

#endif // POP3CLIENT_H
