#ifndef TEXT_H
#define TEXT_H

#include <QtGlobal>
class QChar;
class QFont;
class QFontMetrics;
class QString;
namespace mi
{
namespace opengl
{
class TextPrivate;
class Text
{
public:
Text(const QFont &f);
virtual ~Text();
QFont font() const;
QFontMetrics fontMetrics() const;
void renderText(float x, float y, const QString &text);
private:
Q_DISABLE_COPY(Text)
TextPrivate *const d;
};
} // namespace opengl
} // namespace mi

#endif // TEXT_H
