/*
 * GooCanvas. Copyright (C) 2005-6 Damon Chaplin.
 * Released under the GNU LGPL license. See COPYING for details.
 *
 * goocanvasitemsimple.h - abstract base class for simple items.
 */
#ifndef __GOO_CANVAS_ITEM_SIMPLE_H__
#define __GOO_CANVAS_ITEM_SIMPLE_H__

#include <gtk/gtk.h>
#include "goocanvasitem.h"
#include "goocanvasstyle.h"
#include "goocanvasutils.h"

G_BEGIN_DECLS


#define GOO_TYPE_CANVAS_ITEM_SIMPLE            (goo_canvas_item_simple_get_type ())
#define GOO_CANVAS_ITEM_SIMPLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOO_TYPE_CANVAS_ITEM_SIMPLE, GooCanvasItemSimple))
#define GOO_CANVAS_ITEM_SIMPLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GOO_TYPE_CANVAS_ITEM_SIMPLE, GooCanvasItemSimpleClass))
#define GOO_IS_CANVAS_ITEM_SIMPLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOO_TYPE_CANVAS_ITEM_SIMPLE))
#define GOO_IS_CANVAS_ITEM_SIMPLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GOO_TYPE_CANVAS_ITEM_SIMPLE))
#define GOO_CANVAS_ITEM_SIMPLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GOO_TYPE_CANVAS_ITEM_SIMPLE, GooCanvasItemSimpleClass))


typedef struct _GooCanvasItemSimple       GooCanvasItemSimple;
typedef struct _GooCanvasItemSimpleClass  GooCanvasItemSimpleClass;


/**
 * GooCanvasItemSimple
 * @canvas: the canvas.
 * @parent: the parent item.
 * @bounds: the bounds of the item, in device space.
 * @need_update: if the item needs to recompute its bounds and redraw.
 * @need_entire_subtree_update: if all descendants need to be updated.
 * @style: the style to draw the item with.
 * @transform: the transformation matrix of the item, or %NULL.
 * @clip_path_commands: an array of #GooCanvasPathCommand specifying the clip
 *  path of the item, or %NULL.
 * @tooltip: the item's tooltip.
 * @visibility_threshold: the threshold scale setting at which to show the item
 *  (if the @visibility setting is set to %VISIBLE_ABOVE_THRESHOLD).
 * @visibility: the #GooCanvasItemVisibility setting specifying whether the
 *  item is visible, invisible, or visible above a given canvas scale setting.
 * @pointer_events: the #GooCanvasPointerEvents setting specifying the events
 *  the item should receive.
 * @can_focus: if the item can take the keyboard focus.
 * @clip_fill_rule: the #cairo_fill_rule_t setting specifying the fill rule
 *  used for the clip path.
 * @is_static: if the item is static.
 *
 * The #GooCanvasItemSimple-struct struct contains the basic data needed to
 * implement canvas items.
 */
struct _GooCanvasItemSimple
{
  /* <private> */
  GooCanvasItem parent_object;

  /* <public> */
  GooCanvas *canvas;
  GooCanvasItem *parent;
  GooCanvasStyle *style;
  cairo_matrix_t *transform;
  GArray *clip_path_commands;
  gchar *tooltip;

  /* An array of pointers to GooCanvasItems. The first element is at the
     bottom of the display stack and the last element is at the top. */
  GPtrArray *children;

  GooCanvasBounds bounds;

  gdouble visibility_threshold;

  guint	need_update			: 1;
  guint need_entire_subtree_update      : 1;
  guint visibility			: 2;
  guint pointer_events			: 4;
  guint can_focus                       : 1;
  guint clip_fill_rule			: 4;
  guint is_static			: 1;
};

/**
 * GooCanvasItemSimpleClass
 * @simple_create_path: simple subclasses that draw basic shapes and paths only
 *  need to override this one method. It creates the path for the item.
 *  All updating, painting and hit-testing is provided automatically by the
 *  #GooCanvasItemSimple class. (This method is used by the builtin
 *  #GooCanvasEllipse, #GooCanvasRect and #GooCanvasPath items.)
 *  More complicated subclasses must override @simple_update, @simple_paint and
 *  @simple_is_item_at instead.
 * @simple_update: subclasses should override this to calculate their new
 *  bounds, in user space.
 * @simple_paint: subclasses should override this to paint their item.
 * @simple_is_item_at: subclasses should override this to do hit-testing.
 *
 * The #GooCanvasItemSimpleClass-struct struct contains several methods that
 * subclasses can override.
 *
 * Simple items need only implement the create_path() method. More complex
 * items must override the update(), paint() and is_item_at() methods instead.
 */
struct _GooCanvasItemSimpleClass
{
  /*< private >*/
  GooCanvasItemClass parent_class;

  /*< public >*/
  void		 (* simple_create_path)	(GooCanvasItemSimple   *simple,
					 cairo_t               *cr);

  void           (* simple_update)	(GooCanvasItemSimple   *simple,
					 cairo_t               *cr);
  void           (* simple_paint)	(GooCanvasItemSimple   *simple,
					 cairo_t               *cr,
					 const GooCanvasBounds *bounds);
  gboolean       (* simple_is_item_at)  (GooCanvasItemSimple   *simple,
					 gdouble                x,
					 gdouble                y,
					 cairo_t               *cr,
					 gboolean               is_pointer_event);

  /*< private >*/

  /* Padding for future expansion */
  void (*_goo_canvas_reserved1) (void);
  void (*_goo_canvas_reserved2) (void);
  void (*_goo_canvas_reserved3) (void);
  void (*_goo_canvas_reserved4) (void);
};


GType    goo_canvas_item_simple_get_type		(void) G_GNUC_CONST;

void     goo_canvas_item_simple_set_style		(GooCanvasItemSimple   *simple,
							 GooCanvasStyle        *style);

void     goo_canvas_item_simple_get_path_bounds		(GooCanvasItemSimple	*item,
							 cairo_t		*cr,
							 GooCanvasBounds	*bounds,
							 gboolean                add_tolerance);
void     goo_canvas_item_simple_user_bounds_to_device	(GooCanvasItemSimple	*item,
							 cairo_t		*cr,
							 GooCanvasBounds	*bounds);
void     goo_canvas_item_simple_user_bounds_to_parent	(GooCanvasItemSimple	*item,
							 cairo_t		*cr,
							 GooCanvasBounds	*bounds);
gboolean goo_canvas_item_simple_check_in_path		(GooCanvasItemSimple	*item,
							 gdouble		 x,
							 gdouble		 y,
							 cairo_t		*cr,
							 GooCanvasPointerEvents  pointer_events,
							 gboolean                add_tolerance);
void     goo_canvas_item_simple_paint_path		(GooCanvasItemSimple	*item,
							 cairo_t		*cr,
							 gboolean		 add_tolerance);

void     goo_canvas_item_simple_changed			(GooCanvasItemSimple	*item,
							 gboolean		 recompute_bounds);
gdouble  goo_canvas_item_simple_get_line_width		(GooCanvasItemSimple    *item);

gboolean goo_canvas_item_simple_set_stroke_options	(GooCanvasItemSimple    *simple,
							 cairo_t                *cr,
							 gboolean		 add_tolerance);
gboolean goo_canvas_item_simple_set_fill_options	(GooCanvasItemSimple    *simple,
							 cairo_t                *cr);


G_END_DECLS

#endif /* __GOO_CANVAS_ITEM_SIMPLE_H__ */
