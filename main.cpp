#include <QCoreApplication>
#include <QDir>
#include <QtDebug>

///
/// \brief Generates a file with marked-for-translation strings
/// \param context   : The category to put the strings under, to be shown in Linguist.
///                    Ex. Collections, Fabrics, Machines...
/// \param filePath  : The path to save the file
/// \param names     : The list of the names to be translated
///
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

void generateFabrics() {
  QString collectionPath = "/Users/efstragm/Developer/Drawings/DataFiles/Fabrics";
  QString filePath       = "/Users/efstragm/Developer/Drawings/DraWings/fabricnames.h";

  QDir fabricsDir(collectionPath);
  auto fabricFullNames = fabricsDir.entryInfoList({ "*.fabric" }, QDir::Files);

  QStringList fabricNames;

  std::transform(
      fabricFullNames.cbegin(), fabricFullNames.cend(), std::back_inserter(fabricNames), [](const QFileInfo &info) {
        auto name = info.baseName();
        return name.replace("_", " ").trimmed();
      });

  generate("Fabrics", filePath, fabricNames);
}

void generateCollections() {
  QString collectionPath = "/Users/efstragm/Developer/QtSnap/mydesigns";
  QString filePath       = "/Users/efstragm/Developer/QtSnap/collectionnames.cpp";

  QDir collectionsDir(collectionPath);
  auto collectionNames = collectionsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  generate("Collections", filePath, collectionNames);
}


int main(int argc, char *argv[]) {
  //  generateCollections()
  generateFabrics();
}


