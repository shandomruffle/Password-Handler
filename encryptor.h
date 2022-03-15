#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <QCryptographicHash>
#include <QString>

class Encryptor {
public:
    void Init(const QString& passoword);
    void Reset();
    QString Read(QDataStream& input);
    void Write(QDataStream& output, const QString& string);

private:
    uint8_t GetNextByte();
    QByteArray data_;
    int current_{};

    static inline QCryptographicHash hasher{QCryptographicHash::Algorithm::Sha256};
    static constexpr int kBufferSize = 42;
};

#endif  // ENCRYPTOR_H
