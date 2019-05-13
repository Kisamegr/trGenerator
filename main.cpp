#include <QCoreApplication>
#include <QDir>
#include <QtDebug>

int main(int argc, char *argv[])
{

  QString collectionPath = "/Users/efstragm/Developer/QtSnap/mydesigns";
  QString filePath = "/Users/efstragm/Developer/QtSnap/collectionnames.cpp";

  QFile outFile(filePath);

  if(outFile.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream outStream(&outFile);
    QDir collectionsDir(collectionPath);

    outStream << "// Auto-generated file" << endl;
    outStream << "#include <QtGlobal>" << endl << endl;
    outStream << "const char** collectionNames() {" << endl;
    outStream << "  static const char *names[] = {" << endl;

    QFileInfoList collectionInfoList = collectionsDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i=0; i<collectionInfoList.length(); i++) {
      QFileInfo collection = collectionInfoList[i];
      outStream << "    QT_TRANSLATE_NOOP(\"Collections\", ";
      outStream << "\"" << collection.fileName() << "\")";
      if(i < collectionInfoList.length()-1)
        outStream << ",";
      outStream << endl;
    }

    outStream << "  };" << endl;
    outStream << "  return names;" << endl;
    outStream << "}";

    outStream.flush();
    outFile.close();
  }

}
