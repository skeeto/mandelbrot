/* Copyright (C) 2007  Andy Owen
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as 
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Andy Owen: andy-<YOUR NAME HERE>@ultra-premium.com
 * http://ultra-premium.com/b
 */

// Caller is responsible for freeing (*data)
// If file isn't opened, then *data will be NULL
void readPnm (char *filename, int *width, int *height, unsigned char **data);
