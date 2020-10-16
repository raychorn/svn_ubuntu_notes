#!/bin/bash

svn checkout --non-interactive --username molten_deploy --password peekab00 https://svn.dyn-o-saur.com:8443/svn/repo1/deployments/django_projects/vyperlogix_site $HOME/vyperlogix_site <<< "p"

