# QMake functions for working with QT_VERSION

!isEmpty(QTVERSION_PRI_INCLUDED): error("qtversion.pri already included")
QTVERSION_PRI_INCLUDED = 1

greaterThan(QT_MAJOR_VERSION, 4) {
    DEFINES += QT5
}

# minQtVersion(maj, min, patch): returns true if QT_VERSION is greater than maj.min.patch
defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}
