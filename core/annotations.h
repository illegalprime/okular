/***************************************************************************
 *   Copyright (C) 2005 by Enrico Ros <eros.kde@email.it>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KPDF_ANNOTATIONS_H_
#define _KPDF_ANNOTATIONS_H_

#include <qstring.h>
#include <qdatetime.h>
#include <qpoint.h>
#include <qvaluelist.h>
#include "page.h"

/**
 * @short Base options for an annotation (highlight, stamp, boxes, ..).
 *
 * From PDFreferece v.1.6:
 *  An annotation associates an object such as a note, sound, or movie with a
 *  location on a page of a PDF document ...
 *
 * Inherited classes must modify protected variables as appropriate.
 * Other fields in pdf reference we dropped here:
 *   -subtype, rectangle(we are a rect), border stuff
 */
class Annotation : public NormalizedRect
{
    public:
        Annotation();
        virtual ~Annotation();

        enum State { NewBorn, Creating, Modifying, Closed, Opened };
        enum MouseEvent { MousePress, MouseMove, MouseRelease };
        //enum MouseState { Normal, Hovered, Pressed };
        enum Flags { Hidden, NoOpenable, Print, Locked, ReadOnly };

        // provide some i18n strings
        virtual QString usageTip() const = 0;

        // event handlers (must update state)
        virtual bool mouseEvent( MouseEvent e, double x, double y, Qt::ButtonState b ) = 0;

        // paint roughtly over a cleared area
        virtual void paintOverlay( QPainter * painter, int xScale, int yScale, const QRect & limits ) = 0;
        // cool-paint over a pixmap
        virtual void paintFinal( QImage & backImage, int xScale, int yScale, const QRect & limits ) = 0;

        State state() const { return m_state; }
        const QString & text() const { return m_text; }
        const QString & uniqueName() const { return m_uniqueName; }
        const QDateTime & creationDate() const { return m_creationDate; }
        const QDateTime & modifyDate() const { return m_modifyDate; }
        const QColor & baseColor() const { return m_baseColor; }

        void setBaseColor( const QColor & color ) { m_baseColor = color; }

    protected:
        State m_state;
        //MouseState m_mouseState;
        QString m_text;
        QString m_uniqueName;
        QDateTime m_modifyDate;
        QColor m_baseColor;

    private:
        QDateTime m_creationDate;
};


struct FloatPoint {
    double x, y;
};

class TextAnnotation : public Annotation
{
    public:
        enum Type { InPlace, Popup };
        TextAnnotation( Type type );

        // [Annotation] inherited methods
        QString usageTip() const;
        bool mouseEvent( MouseEvent e, double x, double y, Qt::ButtonState b );
        void paintOverlay( QPainter * painter, int xScale, int yScale, const QRect & limits );
        void paintFinal( QImage & backImage, int xScale, int yScale, const QRect & limits );

    private:
        Type m_type;
};

class LineAnnotation : public Annotation
{
    public:
        enum Type { Line, DestArrow, SrcArrow, BiArrow };
        LineAnnotation( Type type );

        // [Annotation] inherited methods
        QString usageTip() const;
        bool mouseEvent( MouseEvent e, double x, double y, Qt::ButtonState b );
        void paintOverlay( QPainter * painter, int xScale, int yScale, const QRect & limits );
        void paintFinal( QImage & backImage, int xScale, int yScale, const QRect & limits );

    private:
        Type m_type;
};

class GeomAnnotation : public Annotation
{
    //Square, Circle, Polygon
};

class PathAnnotation : public Annotation
{
    //Ink (one or more disjoints paths), Polygon, PolyLine
    public:
        enum Type { Ink, PolyLine };
        PathAnnotation( Type type );

        // [Annotation] inherited methods
        QString usageTip() const;
        bool mouseEvent( MouseEvent e, double x, double y, Qt::ButtonState b );
        void paintOverlay( QPainter * painter, int xScale, int yScale, const QRect & limits );
        void paintFinal( QImage & backImage, int xScale, int yScale, const QRect & limits );

    private:
        Type m_type;
        QValueList<FloatPoint> m_points;
};

class HighlightAnnotation : public Annotation
{
    public:
        enum Type { Highlight, Underline, Squiggly, StrikeOut, BLOCK };
        HighlightAnnotation( Type type );

        // [Annotation] inherited methods
        QString usageTip() const;
        bool mouseEvent( MouseEvent e, double x, double y, Qt::ButtonState b );
        void paintOverlay( QPainter * painter, int xScale, int yScale, const QRect & limits );
        void paintFinal( QImage & backImage, int xScale, int yScale, const QRect & limits );

    private:
        Type m_type;
        QValueList<FloatPoint> m_points;
};

class StampAnnotation : public Annotation
{
    // (14 default symbols + ours)
};

class MediaAnnotation : public Annotation
{
    //FileAttachment, Sound, Movie
    enum Type { FileAttachment, Sound, Movie };
};

#endif
