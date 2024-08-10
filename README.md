QString sanitizeString(const QString &input) {
    // Определяем регулярное выражение для поиска всех символов, кроме разрешённых
    QRegularExpression re(QStringLiteral("[^а-яА-ЯёЁ0-9\\-\\+\\.\\,\\s]"));
        QString sanitizedString = input;
    sanitizedString.replace(re, " ");
    
    return sanitizedString;
}
