#pragma once

#include <QString>
#include <QNetworkReply>

namespace NetworkHelper
{
    static QString networkErrorToString(QNetworkReply::NetworkError error) {
        switch (error) {
            case QNetworkReply::NoError:
                return "No error";
            case QNetworkReply::ConnectionRefusedError:
                return "Connection refused error";
            case QNetworkReply::RemoteHostClosedError:
                return "Remote host closed error";
            case QNetworkReply::HostNotFoundError:
                return "Host not found error";
            case QNetworkReply::TimeoutError:
                return "Timeout error";
            case QNetworkReply::OperationCanceledError:
                return "Operation canceled error";
            case QNetworkReply::SslHandshakeFailedError:
                return "SSL handshake failed error";
            case QNetworkReply::TemporaryNetworkFailureError:
                return "Temporary network failure error";
            case QNetworkReply::ProxyConnectionRefusedError:
                return "Proxy connection refused error";
            case QNetworkReply::ProxyConnectionClosedError:
                return "Proxy connection closed error";
            case QNetworkReply::ProxyNotFoundError:
                return "Proxy not found error";
            case QNetworkReply::ProxyTimeoutError:
                return "Proxy timeout error";
            case QNetworkReply::ProxyAuthenticationRequiredError:
                return "Proxy authentication required error";
            case QNetworkReply::ContentAccessDenied:
                return "Content access denied error";
            case QNetworkReply::ContentOperationNotPermittedError:
                return "Content operation not permitted error";
            case QNetworkReply::ContentNotFoundError:
                return "Content not found error";
            case QNetworkReply::AuthenticationRequiredError:
                return "Authentication required error";
            case QNetworkReply::ContentReSendError:
                return "Content resend error";
            case QNetworkReply::ProtocolUnknownError:
                return "Protocol unknown error";
            case QNetworkReply::ProtocolInvalidOperationError:
                return "Protocol invalid operation error";
            case QNetworkReply::UnknownNetworkError:
                return "Unknown network error";
            case QNetworkReply::UnknownProxyError:
                return "Unknown proxy error";
            case QNetworkReply::UnknownContentError:
                return "Unknown content error";
            case QNetworkReply::ProtocolFailure:
                return "Protocol failure";
            default:
                return "Unknown error";
        }
    }
}
