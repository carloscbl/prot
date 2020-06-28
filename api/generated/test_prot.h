// generated by /usr/local/bin/sqlpp11-ddl2cpp ../db_schemas/prot/prot.sql test_prot test_prot
#ifndef TEST_PROT_TEST_PROT_H
#define TEST_PROT_TEST_PROT_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace test_prot
{
  namespace FormSessions_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct UnqName
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "unq_name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T unqName;
            T& operator()() { return unqName; }
            const T& operator()() const { return unqName; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
    struct Json
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "json";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T json;
            T& operator()() { return json; }
            const T& operator()() const { return json; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct UserForms
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "user_forms";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T userForms;
            T& operator()() { return userForms; }
            const T& operator()() const { return userForms; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace FormSessions_

  struct FormSessions: sqlpp::table_t<FormSessions,
               FormSessions_::Id,
               FormSessions_::UnqName,
               FormSessions_::Json,
               FormSessions_::UserForms>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "form_sessions";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T formSessions;
        T& operator()() { return formSessions; }
        const T& operator()() const { return formSessions; }
      };
    };
  };
  namespace Forms_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct Json
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "json";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T json;
            T& operator()() { return json; }
            const T& operator()() const { return json; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct Name
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
    struct Developer
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "developer";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T developer;
            T& operator()() { return developer; }
            const T& operator()() const { return developer; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct IdPublic
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id_public";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idPublic;
            T& operator()() { return idPublic; }
            const T& operator()() const { return idPublic; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::tinyint>;
    };
    struct Disabled
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "disabled";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T disabled;
            T& operator()() { return disabled; }
            const T& operator()() const { return disabled; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::tinyint>;
    };
  } // namespace Forms_

  struct Forms: sqlpp::table_t<Forms,
               Forms_::Id,
               Forms_::Json,
               Forms_::Name,
               Forms_::Developer,
               Forms_::IdPublic,
               Forms_::Disabled>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "forms";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T forms;
        T& operator()() { return forms; }
        const T& operator()() const { return forms; }
      };
    };
  };
  namespace Schedulers_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct User
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "user";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T user;
            T& operator()() { return user; }
            const T& operator()() const { return user; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace Schedulers_

  struct Schedulers: sqlpp::table_t<Schedulers,
               Schedulers_::Id,
               Schedulers_::User>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "schedulers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T schedulers;
        T& operator()() { return schedulers; }
        const T& operator()() const { return schedulers; }
      };
    };
  };
  namespace Taskers_
  {
    struct Idtasker
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtasker";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtasker;
            T& operator()() { return idtasker; }
            const T& operator()() const { return idtasker; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct User
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "user";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T user;
            T& operator()() { return user; }
            const T& operator()() const { return user; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace Taskers_

  struct Taskers: sqlpp::table_t<Taskers,
               Taskers_::Idtasker,
               Taskers_::User>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "taskers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T taskers;
        T& operator()() { return taskers; }
        const T& operator()() const { return taskers; }
      };
    };
  };
  namespace Tasks_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct Name
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "name";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T name;
            T& operator()() { return name; }
            const T& operator()() const { return name; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
    struct Group
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "!group";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T group;
            T& operator()() { return group; }
            const T& operator()() const { return group; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
    struct Json
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "json";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T json;
            T& operator()() { return json; }
            const T& operator()() const { return json; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct Start
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "start";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T start;
            T& operator()() { return start; }
            const T& operator()() const { return start; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
    struct End
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "end";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T end;
            T& operator()() { return end; }
            const T& operator()() const { return end; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::time_point, sqlpp::tag::can_be_null>;
    };
    struct ConfirmedDone
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "confirmed_done";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T confirmedDone;
            T& operator()() { return confirmedDone; }
            const T& operator()() const { return confirmedDone; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::tinyint, sqlpp::tag::can_be_null>;
    };
    struct ExternalId
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "external_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T externalId;
            T& operator()() { return externalId; }
            const T& operator()() const { return externalId; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
    struct FromUserFormsId
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "from_user_forms_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T fromUserFormsId;
            T& operator()() { return fromUserFormsId; }
            const T& operator()() const { return fromUserFormsId; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct ProtId
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "prot_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T protId;
            T& operator()() { return protId; }
            const T& operator()() const { return protId; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
  } // namespace Tasks_

  struct Tasks: sqlpp::table_t<Tasks,
               Tasks_::Id,
               Tasks_::Name,
               Tasks_::Group,
               Tasks_::Json,
               Tasks_::Start,
               Tasks_::End,
               Tasks_::ConfirmedDone,
               Tasks_::ExternalId,
               Tasks_::FromUserFormsId,
               Tasks_::ProtId>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "tasks";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T tasks;
        T& operator()() { return tasks; }
        const T& operator()() const { return tasks; }
      };
    };
  };
  namespace TasksSchedulers_
  {
    struct Idtask
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtask";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtask;
            T& operator()() { return idtask; }
            const T& operator()() const { return idtask; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Idscheduler
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idscheduler";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idscheduler;
            T& operator()() { return idscheduler; }
            const T& operator()() const { return idscheduler; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace TasksSchedulers_

  struct TasksSchedulers: sqlpp::table_t<TasksSchedulers,
               TasksSchedulers_::Idtask,
               TasksSchedulers_::Idscheduler>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "tasks_schedulers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T tasksSchedulers;
        T& operator()() { return tasksSchedulers; }
        const T& operator()() const { return tasksSchedulers; }
      };
    };
  };
  namespace TasksTaskers_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct Idtask
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtask";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtask;
            T& operator()() { return idtask; }
            const T& operator()() const { return idtask; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Idtasker
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idtasker";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idtasker;
            T& operator()() { return idtasker; }
            const T& operator()() const { return idtasker; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace TasksTaskers_

  struct TasksTaskers: sqlpp::table_t<TasksTaskers,
               TasksTaskers_::Id,
               TasksTaskers_::Idtask,
               TasksTaskers_::Idtasker>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "tasks_taskers";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T tasksTaskers;
        T& operator()() { return tasksTaskers; }
        const T& operator()() const { return tasksTaskers; }
      };
    };
  };
  namespace Users_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct Username
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "username";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T username;
            T& operator()() { return username; }
            const T& operator()() const { return username; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::require_insert>;
    };
    struct Json
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "json";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T json;
            T& operator()() { return json; }
            const T& operator()() const { return json; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
  } // namespace Users_

  struct Users: sqlpp::table_t<Users,
               Users_::Id,
               Users_::Username,
               Users_::Json>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "users";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T users;
        T& operator()() { return users; }
        const T& operator()() const { return users; }
      };
    };
  };
  namespace UsersForms_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update>;
    };
    struct Iduser
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "iduser";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T iduser;
            T& operator()() { return iduser; }
            const T& operator()() const { return iduser; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct Idform
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "idform";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T idform;
            T& operator()() { return idform; }
            const T& operator()() const { return idform; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
  } // namespace UsersForms_

  struct UsersForms: sqlpp::table_t<UsersForms,
               UsersForms_::Id,
               UsersForms_::Iduser,
               UsersForms_::Idform>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "users_forms";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T usersForms;
        T& operator()() { return usersForms; }
        const T& operator()() const { return usersForms; }
      };
    };
  };
} // namespace test_prot
#endif
