#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QtDebug>
#include <QtSql/QtSql>
#include <iostream>

void generate(const QString context, const QString &filePath, const QStringList &names) {
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

bool validityCheck(QSqlDatabase &db) {
  QSqlQuery query("select * from Machines;", db);
  if (!query.isActive()){
    QSqlError err = query.lastError();
    qDebug() << db.databaseName() << " validity check Failure : " << err.driverText() << ", " << err.databaseText() << ". " << err.text() << ". " << err.nativeErrorCode();
    std::cerr << db.databaseName().toStdString()
                <<" validity check Failure : " << err.driverText().toStdString()
               << ", " << err.databaseText().toStdString() << ". " << err.text().toStdString() << ". "
               << err.nativeErrorCode().toStdString();
    return false;
  }
  query.finish();
  return true;
}


QStringList getDataFromDB(QString dbFName)
{
  QFileInfo info(QDir::fromNativeSeparators(dbFName));
  if(!info.exists()) {
    qDebug() << "Database File does not exist";
    return QStringList();
  }

  QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
  db.setDatabaseName(info.filePath());
  if(!db.open()){
    qDebug() << "Database Failed to open";
    return QStringList();
  }

  if(!validityCheck(db))  {
      return QStringList();
    }

  QStringList results;
  QSqlQuery machibeQuery("select Model from Machines order by Model;", db);
  if(machibeQuery.exec())
  {
    while(machibeQuery.next())
      results << machibeQuery.value(0).toString();

    machibeQuery.finish();
  }

  QSqlQuery companyQuery("select Name from Companies order by Name;", db);
  if(companyQuery.exec())
  {
    while(companyQuery.next())
      results << companyQuery.value(0).toString();

    companyQuery.finish();
  }

  return results;
}


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Generate CPP file to used for translations.");
  parser.addHelpOption();
  parser.addPositionalArgument("Source", "Directory to be parsed for files or DB access for Machine names and companies");
  parser.addPositionalArgument("Output", "Cpp path to be written");

  QCommandLineOption fromDB(QStringList() << "d" << "database", "Input is a database file");
  parser.addOption(fromDB);

  parser.process(a);

  const QStringList args = parser.positionalArguments();
        // source is args.at(0), Output is args.at(1)

  bool isDb = parser.isSet(fromDB);


  QString sourcePath = args.at(0);   // e.g. "/Users/efstragm/Developer/QtSnap/mydesigns";
  QString destFile   = args.at(1);   // e.g. "/Users/efstragm/Developer/QtSnap/collectionnames.cpp";

  QDir collectionsDir(sourcePath);
  QStringList collectionNames;
  if(!isDb){
    collectionNames = collectionsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    generate("Collections", destFile, collectionNames);
  }
  else
  {
    //QSqlDatabase::addDatabase("QSQLITE"); // initialization
    collectionNames = getDataFromDB(sourcePath);
    generate("Machines_Comapnies", destFile, collectionNames);
  }

   return 0;
}


