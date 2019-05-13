#include <QCoreApplication>
#include <QDir>
#include <QtDebug>

static void generate(const QString context, const QString &filePath, const QStringList &names) {
  QFile outFile(filePath);

  if(outFile.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream outStream(&outFile);

    outStream << "// Auto-generated file" << endl;
    outStream << "#include <QtGlobal>" << endl << endl;
    outStream << "const char** names() {" << endl;
    outStream << "  static const char *names[] = {" << endl;

    for(int i=0; i<names.length(); i++) {
      auto name= names[i];

      outStream << QString("    QT_TRANSLATE_NOOP(\"%1\", ").arg(context);
      outStream << QString("\"%1\")").arg(name);
      if(i < names.length()-1)
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

int main(int argc, char *argv[]) {
  QString collectionPath = "/Users/efstragm/Developer/QtSnap/mydesigns";
  QString filePath       = "/Users/efstragm/Developer/QtSnap/collectionnames.cpp";

  QDir collectionsDir(collectionPath);
  auto collectionNames = collectionsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  generate("Collections", filePath, collectionNames);
}


