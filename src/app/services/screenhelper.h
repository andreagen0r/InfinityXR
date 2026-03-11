#pragma once

#include <cmath>

#include <QGuiApplication>
#include <QQmlEngine>
#include <QObject>

namespace qone::infinity {

/// \class ScreenHelper
/// \brief Classe utilitária para gerenciamento e consulta de propriedades de tela.
///
/// Esta classe é exportada para o QML como um Singleton, permitindo o acesso
/// centralizado a informações de hardware da tela, como densidade de pixels.
class ScreenHelper : public QObject {

    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    /// \brief Construtor padrão da classe ScreenHelper.
    /// \param parent Ponteiro para o objeto pai (padrão é nullptr).
    explicit ScreenHelper( QObject* parent = nullptr );

    /// \brief Obtém a densidade de pixels de uma tela específica pelo nome.
    /// \param screenName O identificador da tela (ex: "HDMI-1", "eDP-1").
    /// \return A densidade da tela como um valor double.
    /// \note Este método é invocável via QML devido à macro Q_INVOKABLE.
    [[nodiscard]] Q_INVOKABLE double getDensityByName( const QString& screenName );
};

} // namespace qone::infinity
