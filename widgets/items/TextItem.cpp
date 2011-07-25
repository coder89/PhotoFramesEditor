#include "TextItem.h"
#include "global.h"
#include "KEditFactory.h"

#include <QTimeLine>
#include <QInputMethodEvent>
#include <QtTreePropertyBrowser>
#include <QtColorPropertyManager>

#include <klocalizedstring.h>

#define IS_NULL(node) if (node.isNull()) goto _delete;

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::TextItemPrivate
{
    class TextEditUndoCommand;

    TextItemPrivate(TextItem * item) :
        m_item(item),
        m_cursorIsVisible(false),
        m_cursor_line(0),
        m_cursor_character(0)
    {
    }

    void moveCursorLeft()
    {
        --m_cursor_character;
        if (m_cursor_character < 0)
        {
            --m_cursor_line;
            if (m_cursor_line < 0)
            {
                ++m_cursor_line;
                ++m_cursor_character;
            }
            else
                m_cursor_character = m_string_list.at(m_cursor_line).length();
        }
    }

    void moveCursorRight()
    {
        ++m_cursor_character;
        if (m_cursor_character > m_string_list.at(m_cursor_line).length())
        {
            ++m_cursor_line;
            if (m_cursor_line >= m_string_list.count())
            {
                --m_cursor_line;
                --m_cursor_character;
            }
            else
                m_cursor_character = 0;
        }
    }

    void moveCursorUp()
    {
        --(m_cursor_line);
        if (m_cursor_line < 0)
            m_cursor_line = 0;
        else if (m_cursor_character > m_string_list.at(m_cursor_line).length())
            m_cursor_character = m_string_list.at(m_cursor_line).length();
    }

    void moveCursorDown()
    {
        ++(m_cursor_line);
        if (m_cursor_line >= m_string_list.count())
            --m_cursor_line;
        else if (m_cursor_character > m_string_list.at(m_cursor_line).length())
            m_cursor_character = m_string_list.at(m_cursor_line).length();
    }

    void moveCursorEnd()
    {
        m_cursor_character = m_string_list.at(m_cursor_line).length();
    }

    void moveCursorHome()
    {
        m_cursor_character = 0;
    }

    void removeTextAfter()
    {
        if (m_cursor_character < m_string_list.at(m_cursor_line).length())
            m_string_list[m_cursor_line].remove(m_cursor_character, 1);
        else if (m_string_list.count()-1 > m_cursor_line)
        {
            m_string_list[m_cursor_line].append( m_string_list.at(m_cursor_line+1) );
            m_string_list.removeAt(m_cursor_line+1);
        }
    }

    void removeTextBefore()
    {
        if (m_cursor_character > 0 && m_string_list.at(m_cursor_line).length() >= m_cursor_character)
            m_string_list[m_cursor_line].remove(--m_cursor_character, 1);
        else if (m_cursor_line > 0)
        {
            m_string_list[m_cursor_line-1].append( m_string_list.at(m_cursor_line) );
            --m_cursor_line;
            m_cursor_character = m_string_list.at(m_cursor_line).length();
            m_string_list.removeAt(m_cursor_line+1);
        }
    }

    void addNewLine()
    {
        QString temp;
        if (m_cursor_character < m_string_list.at(m_cursor_line).length())
        {
            temp = m_string_list[m_cursor_line].right(m_string_list.at(m_cursor_line).length()-m_cursor_character);
            m_string_list[m_cursor_line].remove(m_cursor_character, m_string_list.at(m_cursor_line).length()-m_cursor_character+1);
        }
        qDebug() << temp;
        m_cursor_character = 0;
        ++m_cursor_line;
        m_string_list.insert(m_cursor_line, temp);
    }

    void addText(const QString & text)
    {
        if (!text.length())
            return;
        m_string_list[m_cursor_line].insert(m_cursor_character, text);
        m_cursor_character += text.length();
    }

    void closeEditor()
    {
        m_item->clearFocus();
    }

    TextItem * m_item;

    QStringList m_string_list;
    QStringList m_temp_string;

    QPointF m_cursor_point;
    bool m_cursorIsVisible;
    int m_cursor_line;
    int m_cursor_character;

    friend class TextItem;
    friend class TextEditUndoCommand;
};

class KIPIPhotoFramesEditor::TextItemPrivate::TextEditUndoCommand : public QUndoCommand
{
        TextItemPrivate * m_item_p;
        QStringList m_prevText;
        bool done;
    public:
        TextEditUndoCommand(const QStringList & prevText, TextItemPrivate * itemPrivate, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            m_item_p(itemPrivate),
            m_prevText(prevText),
            done(true)
        {}
        virtual void redo();
        virtual void undo();
};
void TextItemPrivate::TextEditUndoCommand::redo()
{
    if (!done)
    {
        QStringList temp = m_item_p->m_string_list;
        m_item_p->m_string_list = m_prevText;
        m_prevText = temp;
        m_item_p->m_item->refresh();
        done = !done;
    }
}
void TextItemPrivate::TextEditUndoCommand::undo()
{
    if (done)
    {
        QStringList temp = m_item_p->m_string_list;
        m_item_p->m_string_list = m_prevText;
        m_prevText = temp;
        m_item_p->m_item->refresh();
        done = !done;
    }
}

TextItem::TextItem(const QString & text, QGraphicsScene * scene) :
    AbstractPhoto(scene),
    d(new TextItemPrivate(this)),
    m_metrics(m_font)
{
    this->setText(text);
    this->setFont(QFont());
    this->setColor(Qt::black);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    qDebug() << m_font.key();
}

void TextItem::focusInEvent(QFocusEvent * event)
{
    if (!this->isSelected())
    {
        this->clearFocus();
        return;
    }
    d->m_temp_string = d->m_string_list;
    this->setCursorPositionVisible(true);
    AbstractPhoto::focusInEvent(event);
    this->setCursor(QCursor(Qt::IBeamCursor));
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void TextItem::focusOutEvent(QFocusEvent * event)
{
    if (d->m_temp_string != d->m_string_list)
    {
        QUndoCommand * undo = new TextItemPrivate::TextEditUndoCommand(d->m_temp_string, d);
        PFE_PostUndoCommand(undo);
    }
    d->m_temp_string.clear();
    this->setCursorPositionVisible(false);
    AbstractPhoto::focusOutEvent(event);
    this->unsetCursor();
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void TextItem::keyPressEvent(QKeyEvent * event)
{
    switch (event->key())
    {
        case Qt::Key_Left:
            d->moveCursorLeft();
            break;
        case Qt::Key_Right:
            d->moveCursorRight();
            break;
        case Qt::Key_Up:
            d->moveCursorUp();
            break;
        case Qt::Key_Down:
            d->moveCursorDown();
            break;
        case Qt::Key_Home:
            d->moveCursorHome();
            break;
        case Qt::Key_End:
            d->moveCursorEnd();
            break;
        case Qt::Key_Delete:
            d->removeTextAfter();
            break;
        case Qt::Key_Backspace:
            d->removeTextBefore();
            break;
        case Qt::Key_Return:
            d->addNewLine();
            break;
        case Qt::Key_Escape:
            d->closeEditor();
            break;
        default:
            d->addText(event->text());
    }
    refresh();
    event->setAccepted(true);
}

void TextItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF p = event->pos();

    // Get clicked line number
    d->m_cursor_line =  p.y() / m_metrics.lineSpacing();
    if (d->m_cursor_line >= d->m_string_list.count())
        d->m_cursor_line = d->m_string_list.count()-1;
    QString currentLine = d->m_string_list.at( d->m_cursor_line );

    // Get clicked char position
    p.setX(p.x()-m_metrics.leftBearing(currentLine.at(0)));
    d->m_cursor_character = p.x() / m_metrics.averageCharWidth();
    if (d->m_cursor_character > currentLine.length())
        d->m_cursor_character = currentLine.length();
    int width = m_metrics.width(currentLine, d->m_cursor_character);
    int leftSpace = 0;
    while (width > p.x() && d->m_cursor_character > 0)
    {
        leftSpace = width - p.x();
        width = m_metrics.width(currentLine, --(d->m_cursor_character));
    }
    int rightSpace = 0;
    while (width < p.x() && d->m_cursor_character < currentLine.length())
    {
        rightSpace = p.x() - width;
        width = m_metrics.width(currentLine, ++(d->m_cursor_character));
    }
    this->update();
}

QColor TextItem::color() const
{
    return m_color;
}

void TextItem::setColor(const QColor & color)
{
    m_color = color;
    this->refresh();
}

QFont TextItem::font() const
{
    return m_font;
}

void TextItem::setFont(const QFont & font)
{
    m_font = font;
    m_metrics = QFontMetrics(m_font);
    this->refresh();
}

QString TextItem::text() const
{
    return d->m_string_list.join("\n");
}

void TextItem::setText(const QString & text)
{
    QString temp = text;
    temp.remove('\t');
    d->m_string_list = temp.split('\n');
    this->refresh();
}

QRectF TextItem::boundingRect() const
{
    return m_complete_path.boundingRect();
}

QPainterPath TextItem::shape() const
{
    return m_complete_path;
}

QPainterPath TextItem::opaqueArea() const
{
    return m_complete_path;
}

bool TextItem::contains(const QPointF & point) const
{
    return m_complete_path.contains(point);
}

void TextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if (!m_text_path.isEmpty())
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillPath(m_text_path, m_color);
        painter->restore();
    }

    if (d->m_cursorIsVisible)
    {
        painter->save();
        painter->setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        painter->setPen(Qt::gray);
        int y = m_metrics.lineSpacing() * d->m_cursor_line;
        int x = 0;
        if ( !d->m_string_list.at(d->m_cursor_line).isEmpty() )
        {
            x = m_metrics.width(d->m_string_list.at(d->m_cursor_line),
                                d->m_cursor_character)
                - m_metrics.leftBearing(d->m_string_list.at(d->m_cursor_line).at(0));
        }
        painter->drawLine(x, y, x, y+m_metrics.lineSpacing());
        painter->restore();
    }
}

void TextItem::refresh()
{
    m_text_path = QPainterPath();
    int i = 1;
    int maxBearing = 0;
    int maxWidth = 0;
    const int lineSpacing = m_metrics.lineSpacing();
    foreach (QString string, d->m_string_list)
    {
        if (string.length())
        {
            int width = m_metrics.width(string);
            int leftBearing = -m_metrics.leftBearing(string.at(0));
            m_text_path.addText(leftBearing,
                                lineSpacing*(i)-m_metrics.descent(),
                                m_font,
                                string);
            if (maxWidth < width)
                maxWidth = width;
            if (maxBearing < leftBearing)
                maxBearing = leftBearing;
        }
        ++i;
    }
    m_complete_path = QPainterPath();
    m_complete_path.addRect(0,
                            0,
                            maxWidth + maxBearing,
                            d->m_string_list.count() * m_metrics.lineSpacing());
    this->prepareGeometryChange();
}

QDomElement TextItem::toSvg(QDomDocument & document) const
{
    QDomElement result = AbstractPhoto::toSvg(document);
    result.setAttribute("class", "TextItem");

    // 'defs' tag
    QDomElement defs = document.createElement("defs");
    defs.setAttribute("class", "data");
    result.appendChild(defs);

    // 'defs'-> pfe:'data'
    QDomElement appNS = document.createElementNS(KIPIPhotoFramesEditor::uri(), "data");
    appNS.setPrefix(KIPIPhotoFramesEditor::name());
    defs.appendChild(appNS);

    // 'defs'-> pfe:'data' -> 'text'
    QDomElement text = document.createElement("text");
    text.appendChild(document.createTextNode(d->m_string_list.join("\n").toUtf8()));
    text.setPrefix(KIPIPhotoFramesEditor::name());
    appNS.appendChild(text);

    // 'defs'-> pfe:'data' -> 'color'
    QDomElement color = document.createElement("color");
    color.setPrefix(KIPIPhotoFramesEditor::name());
    color.setAttribute("red", QString::number(m_color.red()));
    color.setAttribute("green", QString::number(m_color.green()));
    color.setAttribute("blue", QString::number(m_color.blue()));
    appNS.appendChild(color);

    // 'defs'-> pfe:'data' -> 'font'
    QDomElement font = document.createElement("font");
    font.setPrefix(KIPIPhotoFramesEditor::name());
    font.setAttribute("data", m_font.toString());
    appNS.appendChild(font);

    return result;
}

QDomElement TextItem::svgVisibleArea(QDomDocument & document) const
{
    QDomElement element = KIPIPhotoFramesEditor::pathToSvg(m_text_path, document);
    element.setAttribute("fill",QString("#")+QString::number(m_color.red(),16)+QString::number(m_color.green(),16)+QString::number(m_color.blue(),16));
    return element;
}

TextItem * TextItem::fromSvg(QDomElement & element)
{
    TextItem * result = new TextItem();
    if (result->AbstractPhoto::fromSvg(element))
    {
        QDomElement defs = element.firstChildElement("defs");
        while (!defs.isNull() && defs.attribute("class") != "data")
            defs = defs.nextSiblingElement("defs");
        IS_NULL(defs);

        QDomElement data = defs.firstChildElement("data");
        IS_NULL(data);

        // text
        QDomElement text = data.firstChildElement("text");
        IS_NULL(text);
        QDomNode textValue = text.firstChild();
        while (!textValue.isNull() && !textValue.isText())
            textValue = textValue.nextSibling();
        IS_NULL(textValue);
        result->d->m_string_list = textValue.toText().data().split('\n');

        // Color
        QDomElement color = data.firstChildElement("color");
        IS_NULL(color);
        result->m_color = QColor(color.attribute("red").toInt(),
                                 color.attribute("green").toInt(),
                                 color.attribute("blue").toInt());

        // Font
        QDomElement font = data.firstChildElement("font");
        IS_NULL(font);
        result->m_font.fromString(font.attribute("data"));

        result->refresh();
        return result;
    }
_delete:
    delete result;
    return 0;
}

QtAbstractPropertyBrowser * TextItem::propertyBrowser()
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();

    // Color
    QtColorPropertyManager * colorManager = new QtColorPropertyManager(browser);
    KColorEditorFactory * colorFactory = new KColorEditorFactory(browser);
    browser->setFactoryForManager(colorManager, colorFactory);
    QtProperty * colorProperty = colorManager->addProperty(i18n("Text color"));
    colorManager->setValue(colorProperty, m_color);
    browser->addProperty(colorProperty);

    // Font
    QtFontPropertyManager * fontManager = new QtFontPropertyManager(browser);
    KFontEditorFactory * fontFactory = new KFontEditorFactory(browser);
    browser->setFactoryForManager(fontManager, fontFactory);
    QtProperty * fontProperty = fontManager->addProperty(i18n("Font"));
    fontManager->setValue(fontProperty, m_font);
    browser->addProperty(fontProperty);

    return browser;
}

QPainterPath TextItem::getLinePath(const QString & string)
{
    QPainterPath result;
    result.addText(0, 0, m_font, string);
    return result;
}

void TextItem::setCursorPositionVisible(bool isVisible)
{
    d->m_cursorIsVisible = isVisible;
    this->update();
}
