
#include "UmlItem.h"
#include "FileOut.h"
#include "UmlTypeSpec.h"

#include "UmlCom.h"
#include "UmlClass.h"
 UmlItem::~UmlItem() {
}

void UmlItem::xmi(int, char **) {
  UmlCom::trace("Error : must be applied on the project<br>");
}

void UmlItem::write(FileOut & out) {
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

}

void UmlItem::write(FileOut &, bool) {
}

void UmlItem::write_description_properties(FileOut & out) {
  if (! description().isEmpty()) {
    static int rank = 0;
    
    out.indent();
    out << "<ownedComment xmi:type=\"uml:Comment\" xmi:id=\"COMMENT_"
      << ++rank << "\" body=\"";
    out.quote(description());
    out << "\"/>\n";
  }

  if (_gen_extension) {
    QCString ste = stereotype();
    const QDict<QCString> up = properties();    
    QDictIterator<QCString> it(up);
    
    if (it.current()) {
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\">\n";
      
      if (! ste.isEmpty()) {
	_stereotypes[ste].append(this);
	out.indent();
	out << "\t<stereotype name=\"";
	out.quote(ste);
	out << "\"/>\n";
      }
      
      do {
	out.indent();
	out << "\t<taggedValue tag=\"";
	out.quote(it.currentKey());
	out << "\" value=\"";
	out.quote(*(it.current()));
	out << "\"/>\n";
	++it;
      } while (it.current());
      
      out.indent();
      out << "</xmi:Extension>\n";
    }
    else if (! ste.isEmpty()) {
      _stereotypes[ste].append(this);
      out.indent();
      out << "<xmi:Extension extender=\"Bouml\">\n";
      out.indent();
      out << "\t<stereotype name=\"";
      out.quote(ste);
      out << "\"/>\n";
      out.indent();
      out << "</xmi:Extension>\n";
    }
  }
}

void UmlItem::memo_relation(UmlItem * r) {
  parent()->memo_relation(r);
}

void UmlItem::write_multiplicity(FileOut & out, QCString s)
{
  if (!s.isEmpty()) {
    static UmlItem * rank = 0;
    
    QCString min;
    QCString max;
    int index = s.find("..");
    
    if (index != -1) {
      min = s.left(index).stripWhiteSpace();
      max = s.mid(index+2).stripWhiteSpace();
    }
    else
      min = max = s.stripWhiteSpace();
    
    out.indent();
    out << "<lowerValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(++rank, "MULTIPLICITY_");
    out << " value=\"" << min << "\"/>\n";
    
    out.indent();
    out << "<upperValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(++rank, "MULTIPLICITY_");
    out << " value=\"" << max << "\"/>\n";
  }
}

void UmlItem::write_type(FileOut & out, const UmlTypeSpec & t)
{
  if (t.type != 0) {
    out.indent();
    out << "<type";
    out.idref(t.type);
    out << "/>\n";
  }
  else if (!t.explicit_type.isEmpty()) {
    out.indent();
    out << "<type";
    out.idref_datatype(t.explicit_type);
    out << "/>\n";
  }
}

void UmlItem::write_default_value(FileOut & out, QCString v)
{
  if (! v.isEmpty()) {
    if (v[0] == '=') {
      v = v.mid(1);
      if (v.isEmpty())
	return;
    }

    static UmlItem * rank = 0;

    out.indent();
    out << "<defaultValue xmi:type=\"uml:LiteralString\"";
    out.id_prefix(rank, "VALUE_");
    out << " value=\"";
    out.quote(v);
    out << "\"/>\n";
  }
}

bool UmlItem::_gen_views;

Language UmlItem::_lang;

bool UmlItem::_uml_20;

bool UmlItem::_use_profile;

bool UmlItem::_pk_prefix;

bool UmlItem::_vis_prefix;

bool UmlItem::_primitive_type;

bool UmlItem::_linefeed;

bool UmlItem::_gen_extension;

