#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <qobject.h>
#include <qstring.h>

// HandleBase //////////////////////////////////////////////////////////////
class HandlerBase : public QObject {
  Q_OBJECT

public:
  HandlerBase(QObject *parent=0, const char *name=0) 
    : QObject(parent, name) { return; }
  virtual ~HandlerBase(void) { return; }

  virtual QString &Name(void)=0;

public slots:
  virtual void Handle(QString data)=0;

signals:
  void Handled(QString data);
};
// END HandlerBase ///////////////////////////////////////////////////////

// template<class T> Handler : public HandlerBase ///////////////////////
template <class T=void> class Handler : public HandlerBase {
public:
  Handler(T *inst, QString (T::*function)(QString), 
	  QString name, QObject *parent=0);
  virtual ~Handler(void) { d_valid = false; d_function = NULL; d_name = ""; 
			   return; }

  bool IsValid(void) { return d_valid; }
  QString &Name(void) { return d_name; }
  void Handle(QString data);
  static Handler<T> *CreateHandler(T *inst, 
				   QString (T::*function)(QString), 
				   QString name);

private:
  bool d_valid;
  QString d_name;

  QString (T::*d_function)(QString);
  T *d_inst;
};
// END template<class T> Handler: public HandlerBase //////////////////

// template specialization of Handler ::: Handler<void> ///////////////
#if 0
template <> class Handler<void> : public HandlerBase {
public:
  Handler(QString (*function)(QString), QString name, QObject *parent=0);
  virtual ~Handler(void) { d_valid = false; d_function = NULL; d_name = ""; 
			   return; }

  bool IsValid(void) { return d_valid; }
  QString &Name(void) { return d_name; }
  void Handle(QString data);
  static Handler<void> *CreateHandler(QString (*function)(QString), 
				      QString name);

private:
  bool d_valid;
  QString d_name;

  QString (*d_function)(QString);
};
#endif
// END template specialization of Handler ::: Handler<void> ///////////

// template <class T> Handler implementation //////////////////////////
template <class T> Handler<T>::Handler<T>(T *inst, 
				       QString (T::*function)(QString), 
				       QString name, QObject *parent)
  : HandlerBase(parent, name) {
  if(inst == NULL || function == NULL) {
    d_valid = false;
    d_inst = NULL;
    d_function = NULL;
  } else {
    d_valid = true;
    d_inst = inst;
    d_function = function;
  }
  if(name != NULL) {
    d_name = name;
  } else {
    d_valid = false;
  }
  return;
}

template <class T> Handler<T> *Handler<T>::CreateHandler(T *inst, 
					 QString (T::*function)(QString), 
					 QString name) {
  Handler<T> *handler = new Handler<T>(inst, function, name);
  if(handler == NULL) {
    return NULL;
  }

  if(!handler->IsValid()) {
    delete handler;
    return NULL;
  }

  return handler;
}

template <class T> void Handler<T>::Handle(QString data) { 
  if(!d_valid) { return; }
  QString output = (d_inst->*d_function)(data);
  if(output.isNull() || output.isEmpty()) {
    return;
  }

  emit Handled(output);

  return;
}
// END template <class T> Handler implementation //////////////////////

// template specialization of Handler ::: Handler<void> Implementation /////
#if 0
Handler<void>::Handler<void>(QString (*function)(QString), 
		       QString name, QObject *parent) 
  : HandlerBase(parent, name) {
  if(function == NULL) {
    d_valid = false;
    d_function = NULL;
  } else {
    d_valid = true;
    d_function = function;
  }
  if(name != NULL) {
    d_name = name;
  } else {
    d_valid = false;
  }
  return;
}

void Handler<void>::Handle(QString data) { 
  if(!d_valid) { return; }
  QString output = d_function(data);
  if(output.isNull() || output.isEmpty()) {
    return;
  }

  emit Handled(output);

  return;
}

template <> Handler<void> *Handler<void>::CreateHandler(
					    QString (*function)(QString), 
					    QString name) {
  Handler<void> *handler = new Handler<void>(function, name);
  if(handler == NULL) {
    return NULL;
  }

  if(!handler->IsValid()) {
    delete handler;
    return NULL;
  }

  return handler;
}
#endif
// END template specialization of Handler ::: Handler<void> Implementation ///

#endif
