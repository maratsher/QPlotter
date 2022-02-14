#ifndef NAMESPACE_H
#define NAMESPACE_H
#include <QStringList>
namespace base {
enum TYPES
    {
        APPROXIMATION,
        INTERPOLATION
    };

enum METHOD_APP
    {
        LINEAR,
        POLINOM,
        EXPO
    };

enum MATHOD_INTER
    {
        LINEAT_SPLINE,
        NYTOM_POLINOM,
        LANGR_POLINOM,
        CUB_SPLINE
    };

extern const QStringList Ltypes;
extern const QStringList Lmethod_app;
extern const QStringList Lmethod_inter;
extern const QStringList Lcolors;

}
#endif // NAMESPACE_H
