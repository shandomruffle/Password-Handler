#include "encryptor.h"

#include <QCryptographicHash>
#include <iostream>

void Encryptor::Init(const QString& passoword) {
    QByteArray raw = passoword.toUtf8();
    hasher.reset();
    hasher.addData(raw);
    data_ = hasher.result();
    Reset();
}

void Encryptor::Reset() {
    current_ = 0;
}

QString Encryptor::Read(QDataStream& input) {
    QByteArray encoded;
    input >> encoded;
    for (auto& byte : encoded) {
        byte ^= GetNextByte();
    }
    return QString::fromUtf8(encoded);
}

void Encryptor::Write(QDataStream& output, const QString& string) {
    QByteArray encoded = string.toUtf8();
    for (auto& byte : encoded) {
        byte ^= GetNextByte();
    }
    output << encoded;
}

uint8_t Encryptor::GetNextByte() {
    if (current_ == data_.length()) {
        hasher.reset();
        hasher.addData(data_);
        data_.append(hasher.result());
    }
    return data_[current_];
}
